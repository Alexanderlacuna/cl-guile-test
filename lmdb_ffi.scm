(use-modules (ice-9 ffi) (system foreign))

(define-module (lmdb))

;;; Include LMDB header files
(define lmdb-headers
  (c-include "lmdb.h"))