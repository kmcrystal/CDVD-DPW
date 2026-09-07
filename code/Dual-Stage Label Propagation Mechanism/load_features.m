function [X, y] = load_features(mat_file)
% load_features -- read a feature .mat exported by extract_features.py
%
% The file holds one variable `data` = [features, label] ([n x d+1]).
% Labels are binary {0,1}; returns [] for y if the label column is absent.

s = load(mat_file);
if isfield(s, 'data')
    data = s.data;
else
    fns = fieldnames(s);
    data = s.(fns{1});
end
X = data(:, 1:end-1);
y = data(:, end);
y = double(y(:));
if any(~ismember(y, [0 1]))
    warning('load_features:labels', ...
            'Non-binary label column in %s; treating labels as unavailable.', ...
            mat_file);
    y = [];
end
end
