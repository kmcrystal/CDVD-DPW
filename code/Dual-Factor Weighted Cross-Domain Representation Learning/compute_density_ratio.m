function r = compute_density_ratio(Xs, Xt, lambda_r)
% compute_density_ratio -- relative density ratio r(x) = P_T(x)/P_S(x)
% via RuLSIF-style Gaussian basis fitting (paper Eq. 13-14)
%
%   r(x) ~= sum_j theta_j * exp( -||x - c_j||^2 / (2*sigma_rbf^2) )
%
% with
%   {c_j}   : b basis centers sampled from the target domain,
%             b = min(100, |D_T|/10)   (at least 10)
%   sigma_rbf : median pairwise distance between source and target samples
%   theta   : learned by minimizing (Eq. 14)
%               (1/2) theta' H theta - h' theta + lambda_r ||theta||^2
%             s.t. theta >= 0, where
%               H_jk = E_{x~P_S}[ phi_j(x) phi_k(x) ]
%               h_j  = E_{x~P_T}[ phi_j(x) ]
%
% Higher r(x) means the source sample is more relevant to the target
% domain from the global distribution-shift-correction perspective.

if nargin < 3, lambda_r = 0.01; end

n_t = size(Xt, 1);
b = max(10, min(100, floor(n_t / 10)));
b = min(b, n_t);

% basis centers sampled from the target domain
sel = randperm(n_t, b);
C = Xt(sel, :);

% Gaussian width: median pairwise source-target distance.  Estimate the
% median on a deterministic bounded subsample; constructing |S|x|T| is not
% feasible for the 31,802-input experiment reported in the paper.
max_bandwidth_samples = 2000;
s_idx = round(linspace(1, size(Xs,1), min(max_bandwidth_samples, size(Xs,1))));
t_idx = round(linspace(1, size(Xt,1), min(max_bandwidth_samples, size(Xt,1))));
D_st = pdist2(Xs(s_idx,:), Xt(t_idx,:));
sigma_rbf = median(D_st(:));
clear D_st;
sigma_rbf = max(sigma_rbf, 1e-6);

% design matrices Phi(x) over source and target samples
Phi_s = exp(-pdist2(Xs, C).^2 / (2 * sigma_rbf^2));   % [n_s x b]
Phi_t = exp(-pdist2(Xt, C).^2 / (2 * sigma_rbf^2));   % [n_t x b]

H = (Phi_s' * Phi_s) / size(Xs, 1);   % H_jk = E_PS[phi_j phi_k]
h = mean(Phi_t, 1)';                  % h_j  = E_PT[phi_j]

% ridge solution with nonnegativity via projected refinement
theta = (H + lambda_r * eye(b)) \ h;
for it = 1:20
    theta = max(theta, 0);
    active = theta > 0;
    if all(active) || ~any(active), break; end
    theta_a = (H(active, active) + lambda_r * eye(sum(active))) \ h(active);
    theta(active) = theta_a;
    theta(~active) = 0;
    if all(theta_a >= 0), break; end
end
theta = max(theta, 0);

r = Phi_s * theta;    % density-ratio estimates at the source samples
r = max(r, 0);
end
