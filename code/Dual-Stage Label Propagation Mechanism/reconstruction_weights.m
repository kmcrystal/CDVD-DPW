function W = reconstruction_weights(X_target, X_source, k, exclude_self)
% reconstruction_weights -- propagation weight matrix learning (paper Eq. 3)
%
% For each target node x_i, the row w_i is obtained by solving the
% minimum-reconstruction-error problem over its k nearest neighbors
% N(x_i) within the source node group:
%
%   min_{w_i} || x_i - sum_a w_{i,ja} x_{ja} ||^2
%   s.t. w_{i,ja} >= 0,  ja in N(x_i),  sum_a w_{i,ja} = 1
%
% Solved per-row via the regularized local Gram system (LLE-style),
% followed by projection onto the probability simplex to enforce the
% nonnegativity and sum-to-one constraints.
%
% X_target [m x d], X_source [n x d]  ->  W sparse [m x n], rows sum to 1.
% exclude_self = true when target and source are the same set (W_pp),
% so a node never reconstructs from itself.

if nargin < 4, exclude_self = false; end

m = size(X_target, 1);
n = size(X_source, 1);
k = min(k, n - double(exclude_self));
if k < 1, W = sparse(m, n); return; end

kdtree = KDTreeSearcher(X_source);
nn = knnsearch(kdtree, X_target, 'K', k + double(exclude_self));
if exclude_self
    % drop the self-match (first column when target == source)
    cleaned = zeros(m, k);
    for i = 1:m
        row = nn(i, :);
        row(row == i) = [];
        cleaned(i, :) = row(1:k);
    end
    nn = cleaned;
end

rows = zeros(m*k, 1);  cols = zeros(m*k, 1);  vals = zeros(m*k, 1);
ptr = 0;
for i = 1:m
    nbrs = nn(i, :);
    Z = X_source(nbrs, :) - X_target(i, :);   % centered neighbors
    G = Z * Z';                               % local Gram matrix [k x k]
    G = G + eye(k) * (1e-3 * trace(G) / k + 1e-12);  % regularization
    w = G \ ones(k, 1);
    w = simplex_project(w);
    rows(ptr+1:ptr+k) = i;
    cols(ptr+1:ptr+k) = nbrs(:);
    vals(ptr+1:ptr+k) = w;
    ptr = ptr + k;
end
W = sparse(rows, cols, vals, m, n);
end

function w = simplex_project(v)
% Euclidean projection of v onto {w : w >= 0, sum(w) = 1}
% (Duchi et al., 2008)
k = numel(v);
u = sort(v, 'descend');
css = cumsum(u);
idx = find(u - (css - 1) ./ (1:k)' > 0, 1, 'last');
if isempty(idx)
    w = ones(k, 1) / k;
else
    theta = (css(idx) - 1) / idx;
    w = max(v - theta, 0);
end
end
