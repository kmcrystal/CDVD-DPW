function [hcp_idx, hcp_labels, hcp_conf, pal_idx, P0, rho, tau] = ...
    stage1_knn_influence(Xl, yl, Xu, k, opts)
% stage1_knn_influence -- Stage I: KNN influence-based pseudo-labeling
% (paper Section 3.4.2, Eq. 2)
%
% For each unlabeled sample u the Gaussian-weighted class influence is
%
%   Influence_c(u) = sum_{x_i in N_k^c(u)} w(u,x_i) / sum_{x_j in N_k(u)} w(u,x_j)
%   w(u,x_i)       = exp( -d(u,x_i)^2 / sigma_knn^2 )
%
% with d the Euclidean distance and sigma_knn the median pairwise
% distance within L_s. Dual thresholds rho > tau split U_s into:
%   HCP: Influence_c(u) > rho for the dominant class  -> D_h
%   PAL: tau < Influence_c(u) <= rho                  -> Stage II input
%   UL : otherwise                                    -> discarded
%
% Entropy-driven threshold learning (paper Eq. 20) optionally adjusts rho
% (and tau, preserving rho > tau) so the influence-entropy of the
% high-confidence pool tracks a target level H_target:
%   rho_{t+1} = rho_t - eta * sign(H_current - H_target)
%
% Returns:
%   hcp_idx/hcp_labels/hcp_conf : indices into Xu, labels {0,1}, confidence
%   pal_idx                     : indices into Xu of PAL samples
%   P0  [n_pal x 2]             : initial class probability vectors
%                                 [Influence_0(u), Influence_1(u)]
%   rho, tau                    : thresholds actually used

n_u = size(Xu, 1);

%% ---- Gaussian bandwidth: median pairwise distance within L_s ----------
n_l = size(Xl, 1);
if n_l > 2000                       % subsample for tractability
    sel = randperm(n_l, 2000);
    sigma_knn = median(pdist(Xl(sel, :)));
else
    sigma_knn = median(pdist(Xl));
end
sigma_knn = max(sigma_knn, 1e-6);

%% ---- k nearest labeled neighbors and influence scores (Eq. 2) ---------
kdtree = KDTreeSearcher(Xl);
[nn_idx, nn_dist] = knnsearch(kdtree, Xu, 'K', k);

W = exp(-(nn_dist.^2) / (sigma_knn^2));        % [n_u x k]
nbr_labels = yl(nn_idx);                       % [n_u x k], values {0,1}
if size(nbr_labels, 2) == 1, nbr_labels = nbr_labels'; end

total = sum(W, 2);  total(total < eps) = eps;
inf1 = sum(W .* (nbr_labels == 1), 2) ./ total;   % Influence_1(u)
inf0 = 1 - inf1;                                  % Influence_0(u)
influence = [inf0, inf1];                         % [n_u x 2]

[dom_score, dom_class] = max(influence, [], 2);   % dominant class (1->c=0, 2->c=1)

%% ---- Entropy-driven threshold learning (Eq. 20) -----------------------
rho = opts.rho;  tau = opts.tau;
if isfield(opts, 'adaptive_thresholds') && opts.adaptive_thresholds
    p = min(max(influence, eps), 1);
    H_all = -sum(p .* log2(p), 2);                % per-sample influence entropy
    for t = 1:20
        hc_mask = dom_score > rho;
        if ~any(hc_mask), rho = rho - opts.eta; continue; end
        H_current = mean(H_all(hc_mask));
        step = opts.eta * sign(H_current - opts.H_target);
        if abs(step) < eps, break; end
        % More ambiguous accepted evidence requires a stricter (higher)
        % threshold; overly concentrated evidence permits relaxation.
        rho_new = min(max(rho + step, 0.75), 0.98);
        if abs(rho_new - rho) < 1e-6, break; end
        rho = rho_new;
    end
    % keep the empirical rho - tau margin, enforce rho > tau
    tau = min(rho - 0.05, opts.tau + (rho - opts.rho));
end

%% ---- Sample classification --------------------------------------------
hcp_mask = dom_score > rho;
pal_mask = ~hcp_mask & (dom_score > tau);

hcp_idx    = find(hcp_mask);
hcp_labels = dom_class(hcp_idx) - 1;              % back to {0,1}
hcp_conf   = dom_score(hcp_idx);
pal_idx    = find(pal_mask);
P0         = influence(pal_idx, :);               % initial p_u for Stage II
end
