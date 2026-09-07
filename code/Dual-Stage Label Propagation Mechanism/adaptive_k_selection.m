function k_best = adaptive_k_selection(Xl, yl, Xu)
% adaptive_k_selection -- dynamic K-selection (paper Eq. 19, Section 3.5.4)
%
%   k* = argmax_k (1/|U_s|) * sum_u Consistency(N_k(u))
%
% Consistency(N_k(u)) measures label agreement within the k-nearest
% labeled neighborhood of unlabeled sample u: the fraction of the k
% neighbors carrying the neighborhood's majority label. Small k wins in
% sparse regions, larger k in dense areas.
%
% Candidates are searched around k_init = sqrt(|L_s|).

n_l    = size(Xl, 1);
k_init = max(3, round(sqrt(n_l)));
k_min  = max(3, round(k_init / 2));
k_max  = min(n_l - 1, 2 * k_init);
candidates = unique(round(linspace(k_min, k_max, 8)));

% One knnsearch with the largest candidate; prefixes give smaller k.
kdtree = KDTreeSearcher(Xl);
nn_idx = knnsearch(kdtree, Xu, 'K', k_max);

best_score = -inf;
k_best     = k_init;
for k = candidates
    nbr_labels = yl(nn_idx(:, 1:k));            % [n_u x k]
    if size(nbr_labels, 2) == 1, nbr_labels = nbr_labels'; end
    % majority-label agreement per unlabeled sample
    pos_frac    = mean(nbr_labels == 1, 2);
    consistency = max(pos_frac, 1 - pos_frac);
    score = mean(consistency);
    fprintf('  k = %3d : mean consistency = %.4f\n', k, score);
    if score > best_score
        best_score = score;
        k_best     = k;
    end
end
end
