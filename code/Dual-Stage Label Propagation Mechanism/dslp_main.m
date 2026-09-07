function dslp_main(labeled_mat, unlabeled_mat, out_mat)
% dslp_main -- Dual-Stage Label Propagation Mechanism (paper Section 3.4)
%
% Expands the labeled source set L_s to the extended training set
% L_s ∪ D_h ∪ D_p using the abundant unlabeled source samples U_s:
%
%   Stage I  (Section 3.4.2): KNN influence-based pseudo-labeling.
%       Gaussian-weighted class influence over the k nearest labeled
%       neighbors classifies each unlabeled sample as
%         HCP (influence > rho = 0.9)  -> definitive pseudo-label, D_h
%         PAL (tau < influence <= rho) -> soft label kept for Stage II
%         UL  (influence <= tau = 0.7) -> discarded (noise prevention)
%
%   Stage II (Section 3.4.3): multi-source label propagation over a
%       heterogeneous graph with three node types (V_L, V_H, V_P) and
%       four reconstruction-weight matrices (W_pl, W_hl, W_ph, W_pp),
%       updated sequentially with hierarchical priorities
%       alpha > beta > gamma > delta, followed by class mass
%       normalization (Eq. 11). Disambiguated PAL samples form D_p.
%
% Inputs (defaults follow the paper's 30% source-label setting):
%   labeled_mat   : .mat with variable data = [features, label]  (L_s)
%   unlabeled_mat : .mat with variable data = [features, label]  (U_s;
%                   labels used ONLY to report pseudo-label accuracy)
%   out_mat       : output file (default 'extended_training_set.mat')
%
% Output .mat variables:
%   features    [n x d]  extended training set features (L_s ∪ D_h ∪ D_p)
%   labels      [n x 1]  labels in {0,1}
%   provenance  [n x 1]  1 = L_s original, 2 = D_h (HCP), 3 = D_p (PAL)
%   confidence  [n x 1]  label confidence in (0,1]
%
% Adaptive parameter configuration (paper Section 3.5.4) is enabled by
% default: dynamic k selection (Eq. 19) and entropy-driven rho/tau
% adjustment (Eq. 20). Set opts fields to false to use fixed values.

if nargin < 1, labeled_mat   = '30%_labels_features_ext.mat'; end
if nargin < 2, unlabeled_mat = '70%_labels_features_ext.mat'; end
if nargin < 3, out_mat       = 'extended_training_set.mat';   end

rng(42);

%% ---- Options (paper defaults) ----------------------------------------
opts.rho     = 0.9;    % high-confidence threshold (Stage I)
opts.tau     = 0.7;    % partial-label threshold   (Stage I, rho > tau)
opts.alpha   = 0.70;   % labeled -> PAL   (ground-truth supervision)
opts.beta    = 0.45;   % labeled -> HCP   (drift prevention)
opts.gamma   = 0.25;   % HCP     -> PAL   (expanded supervision)
opts.delta   = 0.10;   % PAL     -> PAL   (local peer smoothing)
opts.T_max   = 50;     % maximum propagation iterations
opts.epsilon = 1e-4;   % convergence threshold (Eq. 9)
opts.stage2_stability_window = 3; % label must be stable in final iterations
opts.stage2_accept_conf = [];     % [] -> midpoint of effective rho/tau
opts.stage2_neighbor_agreement = []; % [] -> effective tau
opts.adaptive_k          = true;  % dynamic k selection (Eq. 19)
opts.adaptive_thresholds = true;  % entropy-driven rho/tau (Eq. 20)
opts.H_target            = 0.50;  % target influence entropy
opts.eta                 = 0.01;  % threshold learning rate

%% ---- Load data --------------------------------------------------------
[Xl, yl] = load_features(labeled_mat);
[Xu, yu_true] = load_features(unlabeled_mat);
Xl_raw = Xl;
Xu_raw = Xu;

% joint min-max normalization to [0,1] (consistent feature scale)
Xall = [Xl; Xu];
mn = min(Xall, [], 1);  rg = max(Xall, [], 1) - mn;  rg(rg < eps) = 1;
Xl = (Xl - mn) ./ rg;
Xu = (Xu - mn) ./ rg;

fprintf('L_s: %d labeled, U_s: %d unlabeled, dim = %d\n', ...
        size(Xl,1), size(Xu,1), size(Xl,2));

%% ---- Dynamic k selection (Eq. 19) -------------------------------------
if opts.adaptive_k
    k = adaptive_k_selection(Xl, yl, Xu);
else
    k = max(3, round(sqrt(size(Xl,1))));
end
fprintf('Stage I neighborhood size k = %d\n', k);

%% ---- Stage I: KNN influence-based pseudo-labeling ---------------------
[hcp_idx, hcp_labels, hcp_conf, pal_idx, P0, rho, tau] = ...
    stage1_knn_influence(Xl, yl, Xu, k, opts);

fprintf('\nStage I results (rho = %.3f, tau = %.3f):\n', rho, tau);
fprintf('  HCP (high-confidence): %d\n', numel(hcp_idx));
fprintf('  PAL (partially labeled): %d\n', numel(pal_idx));
fprintf('  UL  (discarded): %d\n', ...
        size(Xu,1) - numel(hcp_idx) - numel(pal_idx));

if ~isempty(yu_true) && ~isempty(hcp_idx)
    hcp_accuracy = mean(hcp_labels == yu_true(hcp_idx));
    fprintf('  HCP pseudo-label accuracy vs. ground truth: %.2f%%\n', 100*hcp_accuracy);
else
    hcp_accuracy = NaN;
end

%% ---- Stage II: multi-source label propagation -------------------------
Xh = Xu(hcp_idx, :);
Xp = Xu(pal_idx, :);

if isempty(pal_idx)
    fprintf('\nNo PAL samples: extended set is L_s ∪ D_h only.\n');
    pal_labels = zeros(0,1);  pal_conf = zeros(0,1);
    resolved_pal_idx = zeros(0,1);
    unresolved_pal_idx = zeros(0,1);
    stage2_accuracy = NaN;
else
    opts.rho_effective = rho;
    opts.tau_effective = tau;
    [pal_labels_all, pal_conf_all, FH_final, resolved_mask, stage2_diag] = ...
        stage2_multisource_propagation( ...
        Xl, yl, Xh, hcp_labels, Xp, P0, k, opts);
    % HCP confidence refreshed by the anchored propagation state
    hcp_conf = max(FH_final, [], 2) ./ max(sum(FH_final, 2), eps);

    pal_labels = pal_labels_all(resolved_mask);
    pal_conf = pal_conf_all(resolved_mask);
    Xp = Xp(resolved_mask, :);
    resolved_pal_idx = pal_idx(resolved_mask);
    unresolved_pal_idx = pal_idx(~resolved_mask);

    fprintf('  Stage-II resolved and accepted: %d / %d PAL (%.2f%%)\n', ...
        sum(resolved_mask), numel(resolved_mask), ...
        100 * sum(resolved_mask) / max(numel(resolved_mask), 1));

    if ~isempty(yu_true)
        if ~isempty(pal_labels)
            stage2_accuracy = mean(pal_labels == yu_true(resolved_pal_idx));
            fprintf('  accepted Stage-II accuracy vs. ground truth: %.2f%%\n', 100*stage2_accuracy);
        else
            stage2_accuracy = NaN;
        end
    else
        stage2_accuracy = NaN;
    end
end

%% ---- Assemble extended training set L_s ∪ D_h ∪ D_p -------------------
% Propagation uses normalized copies for distance calculations, but the
% downstream cross-domain learner must receive the original shared-encoder
% coordinates.  Saving the source-only normalized values would put source
% and target in incompatible scales.
features   = [Xl_raw; Xu_raw(hcp_idx, :); Xu_raw(resolved_pal_idx, :)];
labels     = [yl; hcp_labels; pal_labels];
provenance = [ones(size(Xl,1),1); 2*ones(size(Xh,1),1); 3*ones(size(Xp,1),1)];
confidence = [ones(size(Xl,1),1); hcp_conf; pal_conf];

dlp_stats = struct();
dlp_stats.unlabeled_source_nodes = size(Xu, 1);
dlp_stats.stage1_labeled = numel(hcp_idx);
dlp_stats.stage1_accuracy = hcp_accuracy;
dlp_stats.pal_count = numel(pal_idx);
dlp_stats.stage2_resolved = numel(resolved_pal_idx);
dlp_stats.stage2_accuracy = stage2_accuracy;
dlp_stats.stage2_effective = numel(resolved_pal_idx) / max(numel(pal_idx), 1);
dlp_stats.overall_coverage = (numel(hcp_idx) + numel(resolved_pal_idx)) / ...
                             max(size(Xu, 1), 1);
dlp_stats.k = k;
dlp_stats.rho = rho;
dlp_stats.tau = tau;

save(out_mat, 'features', 'labels', 'provenance', 'confidence', 'dlp_stats');
if exist('stage2_diag', 'var')
    save(out_mat, 'stage2_diag', 'resolved_pal_idx', ...
        'unresolved_pal_idx', '-append');
end
fprintf('\nExtended training set: %d samples (%d L_s + %d D_h + %d D_p)\n', ...
        size(features,1), size(Xl,1), size(Xh,1), size(Xp,1));
fprintf('Saved to %s\n', out_mat);
end
