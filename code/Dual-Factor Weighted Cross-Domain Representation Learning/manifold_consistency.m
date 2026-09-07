function MC = manifold_consistency(Xs, Xt, k_MC, omega)
% manifold_consistency -- MC(x) per paper Section 2.3, Eq. 1
%
% For each source sample x_i, compare the local k_MC-nearest-neighbor
% distance distributions in the source and target feature spaces:
%
%   MC(x)      = omega * MC_scale(x) + (1 - omega) * MC_shape(x)
%   MC_scale   = 1 - |d_bar_s - d_bar_t| / max(d_bar_s, d_bar_t)
%   MC_shape   = 1 - |sigma_s - sigma_t| / max(sigma_s, sigma_t)
%
% where (d_bar, sigma) are the mean and standard deviation of the
% distances to the k_MC nearest neighbors in each domain.
% MC in [0,1]; higher = greater local structural stability across
% domains = better cross-domain transferability.
%
% Paper defaults: k_MC = 15 (fixed structural analysis parameter,
% distinct from the label-propagation k), omega = 0.6.

if nargin < 3, k_MC  = 15;  end
if nargin < 4, omega = 0.6; end

n = size(Xs, 1);
k_s = min(k_MC, n - 1);
k_t = min(k_MC, size(Xt, 1));

% Query only the required neighborhoods.  Full pairwise matrices are
% quadratic in memory and exceed practical limits on the paper's CWE sets.
[~, source_dist] = knnsearch(KDTreeSearcher(Xs), Xs, 'K', k_s + 1);
knn_s = source_dist(:, 2:end); % first match is the sample itself
[~, knn_t] = knnsearch(KDTreeSearcher(Xt), Xs, 'K', k_t);

d_bar_s = mean(knn_s, 2);   sig_s = std(knn_s, 0, 2);
d_bar_t = mean(knn_t, 2);   sig_t = std(knn_t, 0, 2);

den_scale = max(max(d_bar_s, d_bar_t), eps);
MC_scale  = 1 - abs(d_bar_s - d_bar_t) ./ den_scale;

den_shape = max(max(sig_s, sig_t), eps);
MC_shape  = 1 - abs(sig_s - sig_t) ./ den_shape;

MC = omega * MC_scale + (1 - omega) * MC_shape;
MC = min(max(MC, 0), 1);
end
