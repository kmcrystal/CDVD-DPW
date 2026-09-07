function [A, w, result] = metric_transfer(X, y, n_S, w0, param)
% metric_transfer -- dual-factor weighted metric transfer learning
% (paper Section 3.5.3, Eq. 16-18)
%
% Jointly learns a Mahalanobis metric A [d x d] and instance weights w by
% minimizing
%
%   J(A,w) = tr(A'A) + lambda*||w - w0||^2 + mu*[ l_in(A,w) - l_out(A,w) ]
%
%   l_in  = sum_{y_i = y_j}  w_i w_j ||A(x_i - x_j)||^2      (Eq. 17)
%   l_out = sum_{y_i ~= y_j} w_i w_j ||A(x_i - x_j)||^2      (Eq. 18)
%
% subject to sum_{i<=n_S} w_i = n_S and w_i >= 0, where w0 is the
% dual-factor prior w0(x) = r(x) * MC(x).
%
% Alternating optimization (paper):
%   (1) fix w, update A by gradient descent:
%         dJ/dA = 2A + 2*mu * sum_ij w_i w_j delta_ij A (x_i-x_j)(x_i-x_j)'
%       with delta_ij = +1 if y_i = y_j else -1;
%   (2) fix A, update w by projected gradient descent onto
%       {w : sum(w_S) = n_S, w >= 0} (target weights stay fixed at 1).
% Iterate until the objective change < epsilon or T_max iterations.
%
% The pairwise sums run over sampled index pairs (num_constraints) with
% same/different-class balance, which keeps the computation tractable on
% large extended training sets without changing the objective's shape.
%
% Rows of X: [source (L_s ∪ D_h ∪ D_p); target labeled (L_t)].

lambda = param.lambda;  mu = param.mu;
gamma  = param.gamma;   gammaW = param.gammaW;

[n, d] = size(X);
n_T = n - n_S;

%% ---- Sample balanced pairwise constraints -----------------------------
m = param.num_constraints;
Ci = zeros(m, 1);  Cj = zeros(m, 1);  delta = zeros(m, 1);
for kk = 1:m
    i = randi(n);  j = randi(n);
    while j == i, j = randi(n); end
    Ci(kk) = i;  Cj(kk) = j;
    delta(kk) = 2 * (y(i) == y(j)) - 1;        % +1 same class, -1 different
end
% rebalance so same/different pairs contribute equal total mass
n_in = sum(delta == 1);  n_out = sum(delta == -1);
if n_in > 0 && n_out > 0
    delta(delta == -1) = -n_in / n_out;
end
V = X(Ci, :) - X(Cj, :);                       % pair difference vectors [m x d]

%% ---- Alternating optimization -----------------------------------------
A = eye(d);
w = w0(:);
J_prev = inf;
result.fs = [];  result.f1 = [];  result.f2 = [];  result.f3 = [];

tic;
for t = 1:param.T_max
    %% (1) fix w, update A
    pw = w(Ci) .* w(Cj) .* delta;              % w_i * w_j * delta_ij
    % sum_ij pw * A * v_ij v_ij'  ==  A * (V' * diag(pw) * V)
    S = V' * (V .* pw);                        % [d x d]
    gradA = 2 * A + 2 * mu * (A * S);
    A = A - gamma * gradA;

    %% (2) fix A, update w (projected gradient)
    VA  = V * A';                              % A-transformed differences
    dij = sum(VA.^2, 2);                       % ||A(x_i - x_j)||^2 per pair
    zeta = zeros(n, 1);
    contrib_i = w(Cj) .* dij .* delta;
    contrib_j = w(Ci) .* dij .* delta;
    for kk = 1:m
        zeta(Ci(kk)) = zeta(Ci(kk)) + contrib_i(kk);
        zeta(Cj(kk)) = zeta(Cj(kk)) + contrib_j(kk);
    end
    gradW = 2 * lambda * (w - w0(:)) + 2 * mu * zeta;
    w = w - gammaW * gradW;

    % projection: w >= 0, sum of source weights = n_S, target weights = 1
    w = max(w, 0);
    s = sum(w(1:n_S));
    if s > eps, w(1:n_S) = w(1:n_S) * (n_S / s); end
    w(n_S+1:end) = 1;

    %% objective (Eq. 16) and convergence
    pw  = w(Ci) .* w(Cj) .* delta;
    VA  = V * A';
    dij = sum(VA.^2, 2);
    f1 = trace(A' * A);
    f2 = lambda * norm(w - w0(:))^2;
    f3 = mu * sum(pw .* dij);                  % l_in - l_out (signed)
    J  = f1 + f2 + f3;

    result.f1(t) = f1;  result.f2(t) = f2;  result.f3(t) = f3;
    result.fs(t) = J;

    if abs(J_prev - J) < param.epsilon
        fprintf('  converged at iteration %d (dJ = %.2e)\n', t, abs(J_prev - J));
        break;
    end
    J_prev = J;
end
result.time = toc;
result.iter = numel(result.fs);
result.A = A;
result.w = w;
fprintf('  metric transfer finished: %d iterations, %.2fs\n', ...
        result.iter, result.time);
end
