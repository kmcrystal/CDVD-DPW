function [lambda_out, mu_out, mmd] = adaptive_lambda_mu(Xs, Xt, w0_s, ...
                                                        lambda_in, mu_in)
% adaptive_lambda_mu -- metric-learning parameter adaptation (paper Eq. 21)
%
%   lambda / mu = clip( MMD(L_s ∪ D_h ∪ D_p, L_t) / Var(w0), [0.5, 10] )
%
% Large MMD (very different domains)  -> high ratio (~10): anchor the
% learned weights to the dual-factor prior w0, which is computed from
% feature-space geometry and stays reliable under divergence.
% Small MMD (similar domains) -> low ratio (~0.5): allow stronger
% discriminative metric learning through larger mu.
% Var(w0) normalizes for the spread of the dual-factor weights.
%
% The product lambda*mu of the initial setting is preserved; only the
% ratio is adapted.

mmd = compute_MMD(Xs, Xt);

var_w0 = var(w0_s);
if var_w0 < eps, var_w0 = 1; end

ratio = min(max(mmd / var_w0, 0.5), 10);

product    = lambda_in * mu_in;
mu_out     = sqrt(product / ratio);
lambda_out = ratio * mu_out;
end
