function reduced_X = PCA_reduce(X, retain_dimensions)
% PCA_reduce -- project X onto its top principal components.
% Used to reduce the 128-d BiLSTM features before metric learning.
[U, ~, ~] = svd(cov(X));
retain_dimensions = min(retain_dimensions, size(U, 2));
reduced_X = X * U(:, 1:retain_dimensions);
end
