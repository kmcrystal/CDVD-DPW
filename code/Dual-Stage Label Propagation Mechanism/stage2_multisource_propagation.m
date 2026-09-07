function [pal_labels, pal_conf, FH, resolved, diagnostics] = ...
    stage2_multisource_propagation( ...
    Xl, yl, Xh, yh, Xp, P0, k, opts)
% stage2_multisource_propagation -- Stage II: multi-source label
% propagation for disambiguation (paper Section 3.4.3, Eq. 4-11)
%
% Node types:
%   V_L : labeled nodes, F_L fixed one-hot                    (ground truth)
%   V_H : HCP nodes, F_H initialized from Stage I pseudo-labels
%   V_P : PAL nodes, P initialized with Stage I influence scores
%
% Propagation weight matrices (Eq. 3, reconstruction_weights):
%   W_pl : V_L -> V_P   ground-truth supervision of PAL nodes
%   W_hl : V_L -> V_H   anchoring HCP nodes to prevent label drift
%   W_ph : V_H -> V_P   expanding supervision signals via HCP
%   W_pp : V_P -> V_P   local peer smoothing among PAL nodes
% (The reverse path W_hp is deliberately excluded: information flows
%  strictly from reliable to uncertain nodes.)
%
% Sequential update per iteration t (Eq. 4-8), with hierarchical
% priorities alpha > beta > gamma > delta:
%   Step 1: F_P^(t,1) = alpha * W_pl * F_L + (1-alpha) * P
%   Step 2: F_H^(t)   = beta  * W_hl * F_L + (1-beta)  * F_H^(t-1)
%   Step 3: F_P^(t,2) = gamma * W_ph * F_H^(t) + (1-gamma) * F_P^(t,1)
%   Step 4: F_P^(t)   = (delta * W_pp + (1-delta) * I) * F_P^(t,2)
% until ||F_P^(t)-F_P^(t-1)||_F + ||F_H^(t)-F_H^(t-1)||_F < epsilon
% or T_max iterations (Eq. 9).
%
% Final label assignment uses class mass normalization (Eq. 11):
%   y_hat_i = argmax_c (n_c / n_hat_c) * F_P^(T)[i,c]
% where n_c is the class prior in L_s and n_hat_c the predicted mass.

alpha = opts.alpha;  beta = opts.beta;
gamma = opts.gamma;  delta = opts.delta;
assert(alpha > beta && beta > gamma && gamma > delta, ...
       'hierarchical priority requires alpha > beta > gamma > delta');

n_p = size(Xp, 1);
n_h = size(Xh, 1);

% Fixed one-hot confidence of labeled nodes: rows [1-y, y]
FL = [1 - yl, yl];

%% ---- Propagation weight matrices (Eq. 3) ------------------------------
fprintf('Stage II: learning propagation weight matrices (k = %d)...\n', k);
Wpl = reconstruction_weights(Xp, Xl, k);
Wpp = reconstruction_weights(Xp, Xp, k, true);
if n_h > 0
    Whl = reconstruction_weights(Xh, Xl, k);
    Wph = reconstruction_weights(Xp, Xh, k);
    FH  = [1 - yh, yh];                    % F_H^(0): one-hot pseudo-labels
else
    Whl = sparse(0, size(Xl,1));  Wph = sparse(n_p, 0);
    FH  = zeros(0, 2);
end

%% ---- Sequential propagation (Eq. 4-9) ---------------------------------
FP = P0;
history_len = max(1, opts.stage2_stability_window);
label_history = zeros(n_p, history_len);
FP1 = alpha * (Wpl * FL) + (1 - alpha) * P0;   % Step 1 is iteration-invariant
for t = 1:opts.T_max
    FP_prev = FP;  FH_prev = FH;

    % Step 2 -- Labeled -> HCP (drift prevention)
    if n_h > 0
        FH = beta * (Whl * FL) + (1 - beta) * FH_prev;
    end

    % Step 3 -- HCP -> PAL (expanded supervision)
    if n_h > 0
        FP2 = gamma * (Wph * FH) + (1 - gamma) * FP1;
    else
        FP2 = FP1;
    end

    % Step 4 -- PAL self-smoothing (local consistency)
    FP = delta * (Wpp * FP2) + (1 - delta) * FP2;

    % Track calibrated labels for the final-iteration stability criterion.
    q_iter = class_mass_calibrate(FP, yl);
    [~, iter_cls] = max(q_iter, [], 2);
    label_history(:, mod(t - 1, history_len) + 1) = iter_cls - 1;

    % Convergence check (Eq. 9)
    change = norm(FP - FP_prev, 'fro') + norm(FH - FH_prev, 'fro');
    if change < opts.epsilon
        fprintf('  converged at iteration %d (change = %.2e)\n', t, change);
        break;
    end
end

%% ---- Class mass normalization + label assignment (Eq. 10-11) ----------
F_norm = class_mass_calibrate(FP, yl);
[pal_conf, cls] = max(F_norm, [], 2);
pal_labels = cls - 1;                          % back to {0,1}

% Stage II is selective: a PAL node joins D_p only when its calibrated
% confidence, reliable-neighborhood support and final label stability all
% pass.  Otherwise it remains unlabeled, matching the RQ3 definition of
% "successfully disambiguated and accepted".
accept_conf = opts.stage2_accept_conf;
if isempty(accept_conf)
    accept_conf = (opts.rho_effective + opts.tau_effective) / 2;
end
min_agreement = opts.stage2_neighbor_agreement;
if isempty(min_agreement), min_agreement = opts.tau_effective; end

X_reliable = [Xl; Xh];
y_reliable = [yl; yh];
k_rel = min(k, size(X_reliable, 1));
[rel_idx, rel_dist] = knnsearch(KDTreeSearcher(X_reliable), Xp, 'K', k_rel);
if k_rel == 1
    rel_idx = rel_idx(:); rel_dist = rel_dist(:);
end
scale = median(rel_dist(:));
scale = max(scale, 1e-6);
rel_w = exp(-(rel_dist .^ 2) / (scale ^ 2));
rel_labels = y_reliable(rel_idx);
agreement = sum(rel_w .* (rel_labels == pal_labels), 2) ./ ...
            max(sum(rel_w, 2), eps);

filled = min(t, history_len);
recent = label_history(:, 1:filled);
stable = all(recent == recent(:, 1), 2);
resolved = pal_conf >= accept_conf & agreement >= min_agreement & stable;

diagnostics = struct();
diagnostics.total_pal = n_p;
diagnostics.resolved = sum(resolved);
diagnostics.accept_conf = accept_conf;
diagnostics.min_neighbor_agreement = min_agreement;
diagnostics.confidence = pal_conf;
diagnostics.neighbor_agreement = agreement;
diagnostics.stable = stable;
diagnostics.iterations = t;
end

function Q = class_mass_calibrate(FP, yl)
% Equations 12-15: class-mass rescaling followed by row normalization.
prior = [mean(yl == 0), mean(yl == 1)];
predicted = mean(FP, 1); predicted(predicted < eps) = eps;
Q = FP .* (prior ./ predicted);
row_sum = sum(Q, 2); row_sum(row_sum < eps) = eps;
Q = Q ./ row_sum;
end
