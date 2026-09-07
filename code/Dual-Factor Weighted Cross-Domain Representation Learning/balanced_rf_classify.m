function pred = balanced_rf_classify(X, y, w, A, Xtest, n_trees)
% balanced_rf_classify -- Balanced Random Forest (paper Section 3.5.5)
%
% Trains on the metric-aligned representations z_tilde = A * z of the
% extended source set + labeled target set, and predicts the target test
% samples (also transformed with the learned metric).
%
% Class imbalance handling: each tree receives a bootstrap sample with
% equal representation of the vulnerable (1) and non-vulnerable (0)
% classes. Sampling of the majority class is weighted by the learned
% instance weights w, so samples with high dual-factor transfer value
% are preferred. Final prediction is the majority vote of the ensemble.

if nargin < 6, n_trees = 100; end

Xtr = X * A';                 % z_tilde = A * z
Xte = Xtest * A';

minority_idx = find(y == 1);
majority_idx = find(y == 0);
n_min = numel(minority_idx);
assert(n_min > 0 && ~isempty(majority_idx), ...
       'balanced_rf_classify: both classes must be present');

% weighted sampling probabilities within each class
w = max(w(:), eps);
p_min = w(minority_idx) / sum(w(minority_idx));
p_maj = w(majority_idx) / sum(w(majority_idx));

votes = zeros(size(Xte, 1), n_trees);
for t = 1:n_trees
    % balanced bootstrap: n_min from each class
    sel_min = minority_idx(weighted_sample(p_min, n_min));
    sel_maj = majority_idx(weighted_sample(p_maj, n_min));
    idx = [sel_min; sel_maj];

    tree = fitctree(Xtr(idx, :), y(idx), ...
                    'MinLeafSize', 2, ...
                    'MaxNumSplits', 50, ...
                    'SplitCriterion', 'gdi');
    votes(:, t) = predict(tree, Xte);
end
pred = mode(votes, 2);
end

function idx = weighted_sample(p, n)
% sample n indices with replacement according to probability vector p
edges = [0; cumsum(p(:))];
edges(end) = 1;
[~, idx] = histc(rand(n, 1), edges); %#ok<HISTC>
idx = min(max(idx, 1), numel(p));
end
