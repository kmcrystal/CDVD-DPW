function mmd = compute_MMD(Xs, Xt)
% compute_MMD -- Gaussian-kernel Maximum Mean Discrepancy estimate
% between source and target samples (used by adaptive_lambda_mu, Eq. 21).
%
%   MMD^2 = E[k(xs,xs')] - 2 E[k(xs,xt)] + E[k(xt,xt')]
%
% Bandwidth: median source-target pairwise distance (median heuristic).
% Large sets are subsampled for tractability.

max_n = 2000;
if size(Xs,1) > max_n, Xs = Xs(randperm(size(Xs,1), max_n), :); end
if size(Xt,1) > max_n, Xt = Xt(randperm(size(Xt,1), max_n), :); end

D_st = pdist2(Xs, Xt);
sigma = median(D_st(:));
if sigma < eps, sigma = 1; end

Kss = exp(-pdist2(Xs, Xs).^2 / (2 * sigma^2));
Ktt = exp(-pdist2(Xt, Xt).^2 / (2 * sigma^2));
Kst = exp(-D_st.^2 / (2 * sigma^2));

mmd = mean(Kss(:)) - 2 * mean(Kst(:)) + mean(Ktt(:));
mmd = max(mmd, 0);
end
