function [metrics, result] = cdr_main(source_ext_mat, target_labeled_mat, ...
                                      target_test_mat)
% cdr_main -- Dual-Factor Weighted Cross-Domain Representation Learning
% (paper Section 3.5) + Balanced Random Forest classification (3.5.5)
%
% Pipeline:
%   1. Load the extended source training set L_s ∪ D_h ∪ D_p produced by
%      the Dual-Stage Label Propagation module, the labeled target set
%      L_t (default 5% of the target domain) and the target test set.
%   2. Factor 1 -- density ratio r(x) = P_T(x)/P_S(x) via RuLSIF-style
%      Gaussian basis fitting (Eq. 13-14): global distributional relevance.
%   4. Factor 2 -- manifold consistency MC(x) (Eq. 1, k_MC = 15,
%      omega = 0.6): local structural stability across domains.
%   5. Dual-factor prior weight w0(x) = r(x) * MC(x) (Eq. 15).
%   6. Adaptive lambda/mu calibration from MMD (Eq. 21).
%   7. Metric transfer learning (Eq. 16-18): alternating optimization of
%      the Mahalanobis metric A and instance weights w.
%   8. Transform all features z_tilde = A * z; train Balanced Random
%      Forest on the weighted source set; classify target test samples.
%
% Inputs:
%   source_ext_mat     : 'extended_training_set.mat' from DSLP
%                        (features, labels, provenance, confidence)
%   target_labeled_mat : .mat with `data` = [features, label]   (L_t, 5%)
%   target_test_mat    : .mat with `data` = [features, label]   (test set)
%
% Outputs:
%   metrics : struct with precision, recall, f1, mcc, tps, tpn
%   result  : optimization diagnostics (A, w, w0, r, MC, curves)

if nargin < 1, source_ext_mat     = 'extended_training_set.mat'; end
if nargin < 2, target_labeled_mat = '5%_labels_features_ext.mat'; end
if nargin < 3, target_test_mat    = '95%_labels_features_ext.mat'; end

rng(42);
warning('off');

%% ---- 1. Load data -----------------------------------------------------
S = load(source_ext_mat);
Xs = S.features;  ys = S.labels(:);
[Xt,  yt]  = load_features_cdr(target_labeled_mat);
[Xte, yte] = load_features_cdr(target_test_mat);

n_S = size(Xs, 1);  n_T = size(Xt, 1);
fprintf('Source (L_s ∪ D_h ∪ D_p): %d, target labeled L_t: %d, test: %d\n', ...
        n_S, n_T, size(Xte, 1));

%% ---- 2. Shared vulnerability-oriented representation ------------------
param = init_param(size(Xs, 2));
% No unreported domain-specific PCA is applied.  The source and target files
% must have been produced by extract_transfer_features.py, which guarantees
% that all rows already occupy the same 128-dimensional encoder space.
assert(size(Xs,2) == size(Xt,2) && size(Xs,2) == size(Xte,2), ...
       'source and target feature dimensions differ');

%% ---- 3. Factor 1: density ratio r(x) (Eq. 13-14) ----------------------
disp('Computing density ratio r(x) [global distributional relevance]...');
X_target_all = [Xt; Xte];      % all available target samples define P_T
r = compute_density_ratio(Xs, X_target_all, param.lambda_r);
r = r / max(max(r), eps);      % normalize to [0,1] (paper Section 3.5.1)

%% ---- 4. Factor 2: manifold consistency MC(x) (Eq. 1) ------------------
disp('Computing manifold consistency MC(x) [local structural stability]...');
MC = manifold_consistency(Xs, X_target_all, param.k_MC, param.omega);

%% ---- 5. Dual-factor prior weight (Eq. 15) -----------------------------
% Multiplicative conjunction: only samples both globally relevant AND
% locally stable receive high weight.
w0_s = r(:) .* MC(:);
% scale source weights so sum(w_S) = n_S (constraint of Eq. 16)
w0_s = w0_s * (n_S / max(sum(w0_s), eps));
w0   = [w0_s; ones(n_T, 1)];   % target samples fixed at weight 1

%% ---- 6. Adaptive lambda/mu from MMD (Eq. 21) --------------------------
disp('Adaptive lambda/mu calibration via MMD...');
[param.lambda, param.mu, mmd_val] = adaptive_lambda_mu( ...
    Xs, Xt, w0_s, param.lambda, param.mu);
fprintf('  MMD = %.4f  ->  lambda = %.3f, mu = %.3f (ratio %.2f)\n', ...
        mmd_val, param.lambda, param.mu, param.lambda / param.mu);

%% ---- 7. Metric transfer learning (Eq. 16-18) --------------------------
disp('Metric transfer learning (alternating optimization of A and w)...');
X = [Xs; Xt];
y = [ys; yt(:)];
[A, w, result] = metric_transfer(X, y, n_S, w0, param);
result.r  = r;
result.MC = MC;
result.w0 = w0;
result.parameters = param;

try
    writematrix(A, 'A.csv');
catch
    % older MATLAB: csvwrite fallback
    csvwrite('A.csv', A); %#ok<CSWRITE>
end

%% ---- 8. Balanced Random Forest classification (Section 3.5.5) ---------
disp('Balanced Random Forest classification...');
pred = balanced_rf_classify(X, y, w, A, Xte, param.n_trees);
metrics = evaluate_metrics(yte, pred);
result.predictions = pred;
result.ground_truth = yte;

fprintf('\n--- Target-domain test results ---\n');
fprintf('Precision: %.2f%%  Recall: %.2f%%  F1: %.2f%%  MCC: %.4f\n', ...
        100*metrics.precision, 100*metrics.recall, ...
        100*metrics.f1, metrics.mcc);
fprintf('tps (vuln recall): %.4f  tpn (non-vuln recall): %.4f\n', ...
        metrics.tps, metrics.tpn);
end

% ------------------------------------------------------------------------
function param = init_param(input_dim)
% Defaults correspond to the CDVD-DPW paper settings.
param.dim      = input_dim; % shared BiLSTM feature dimension (128)
param.k_MC     = 15;    % manifold-consistency neighborhood (Section 2.3)
param.omega    = 0.6;   % MC scale/shape balance (Eq. 1)
param.lambda_r = 0.01;  % RuLSIF regularization lambda_r (Eq. 14)
param.lambda   = 10;    % weight regularization (rebalanced adaptively)
param.mu       = 1;     % margin maximization   (rebalanced adaptively)
param.gamma    = 1e-3;  % step size for metric A update
param.gammaW   = 1e-4;  % step size for weight w update
param.num_constraints = 2000; % sampled pairwise terms of l_in/l_out
param.epsilon  = 1e-4;  % convergence: objective change threshold
param.T_max    = 100;   % maximum alternating iterations
param.n_trees  = 100;   % Balanced Random Forest size
end

% ------------------------------------------------------------------------
function [X, y] = load_features_cdr(mat_file)
s = load(mat_file);
if isfield(s, 'data'), data = s.data;
else, fns = fieldnames(s); data = s.(fns{1}); end
X = data(:, 1:end-1);
y = double(data(:, end));
end
