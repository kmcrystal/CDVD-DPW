function metrics = evaluate_metrics(y_true, y_pred)
% evaluate_metrics -- Precision, Recall, F1 and MCC (paper Section 4.3)
%
%   Prec = TP/(TP+FP)      Rec = TP/(TP+FN)
%   F1   = 2*Prec*Rec/(Prec+Rec)
%   MCC  = (TP*TN - FP*FN) / sqrt((TP+FP)(TP+FN)(TN+FP)(TN+FN))
%
% MCC in [-1,1] is the primary metric under severe class imbalance.
% Also reports tps = TP/P and tpn = TN/N (per-class recalls).

y_true = y_true(:);  y_pred = y_pred(:);
TP = sum(y_pred == 1 & y_true == 1);
TN = sum(y_pred == 0 & y_true == 0);
FP = sum(y_pred == 1 & y_true == 0);
FN = sum(y_pred == 0 & y_true == 1);

metrics.precision = TP / max(TP + FP, 1);
metrics.recall    = TP / max(TP + FN, 1);
if metrics.precision + metrics.recall > 0
    metrics.f1 = 2 * metrics.precision * metrics.recall / ...
                 (metrics.precision + metrics.recall);
else
    metrics.f1 = 0;
end

den = sqrt(double(TP+FP) * double(TP+FN) * double(TN+FP) * double(TN+FN));
if den > 0
    metrics.mcc = (double(TP)*double(TN) - double(FP)*double(FN)) / den;
else
    metrics.mcc = 0;
end

metrics.tps = TP / max(TP + FN, 1);   % vulnerable-class recall
metrics.tpn = TN / max(TN + FP, 1);   % non-vulnerable-class recall
metrics.confusion = [TP, FN; FP, TN];
end
