(use-modules (system foreign-library)
	         (ice-9 rdelim)
	         (system foreign)
	)

;; get dynmaic link from .sof ile 

;;(load-extension "./libhello" "hello")



(define ptr (foreign-library-pointer "./libhello" "scm_fibonacci"))
(define fibonacci (foreign-library-function "./libhello" "scm_fibonacci"
	#:return-type int
	#:arg-types (list int)
	))

;;expand the aove
(define defer-with-pointer (pointer->procedure   int
              ptr 
              (list int)
              #:return-errno? #t
	)
)

;;using pointe->

(write-line (fibonacci 15))
 
;;(display  (defer-with-pointer 1))
#|


(define hello (dynamic-link "./libhello"))
(define hello2
	(dynamic-func "hello" hello)
	)
;;(dynamic-call "hello" hello)


(define hello (pointer->procedure                                       ; specify return type, here, it's a pointer
                   (foreign-library-function "./libhello" "hello") ; get function pointer you want
                   ))

|#




;;pass pointers to foreign functions and return them as well


(define memcpy 
	(foreign-library-function #f "memcpy"
		                      #:return-type '*
		                      #:arg-types (list '* '* size_t)
		)
	)



;;invoke memcpy

(use-modules (rnrs bytevectors))

(define src-bits
  (u8-list->bytevector '(0 1 2 3 4 5 6 7)))


(define src 
	(bytevector->pointer src-bits)

	)
(define dest
	(bytevector->pointer (make-bytevector  16 0))
	)

(memcpy dest src (bytevector-length src-bits))


(bytevector->u8-list (pointer->bytevector dest 16))


;;todo: Refactor
(define ptr (foreign-library-pointer "./libhello" "myName"))

(define defer-string-pointer (pointer->procedure   '*
              ptr 
              '()
              #:return-errno? #t
	)
)

(define ptr-results (defer-string-pointer))
(define factorial (foreign-library-function "./libhello" "scm_factorial"
	#:return-type int
	#:arg-types (list int)
	))

(define results (parse-c-struct (make-c-struct (list int int)
                               (list 8 11))
                (list int int)))

(define ptr (foreign-library-pointer "./libhello" "struct_getter"))

(define defer-with-pointer (pointer->procedure (list '*)
              ptr 
              (list )
              #:return-errno? #t
	)
)

(define ffi-struct  (defer-with-pointer))

(define results (parse-c-struct ffi-struct (list int int )))
(write-line results)
(define* (ptr->procedure-getter  ffi_symbol #:optional (lib-name "./libhello"))  (foreign-library-pointer lib-name  ffi_symbol))
;; make a remake
(define (struct-deserializer ptr  return_type) (parse-c-struct ptr return-type))
(define (dereference-uint8* ptr)
  ;; Helper function: dereference the byte pointed to by PTR.
  (let ((b (pointer->bytevector ptr 1)))
    (bytevector-u8-ref b 0)))

(display (ptr->procedure-getter  "struct_getter"))