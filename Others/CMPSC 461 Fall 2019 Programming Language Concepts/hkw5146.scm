(define (PAIR a b) (lambda (p) (p a b)))

(define (LEFT g) (g (lambda (t f) t)))

(define (RIGHT p) (p (lambda (t f) f)))

(define (funcompose op)
    (lambda (i) (
        fold-left 
          (lambda (a b) eval(eval(list b a)))
          i 
          (fold-left (lambda (i j) (cons j i)) '() op) )))

(define (merge a b)
    (if (null? a) b
		    (if (null? b) a
			    (if (> (car a) (car b))
				    (cons (car b) (merge (cdr b) a))
				    (cons (car a) (merge (cdr a) b))))))
    
(define (findMax L)
    (cond 
          ((null? L) 0)
          ((null? (cdr L)) (car L))
          ((list? (cadr L)) (findMax(cons (car L) (cons (findMax (cadr L)) (cddr L)))))
          ((< (car L) (cadr L))
            (findMax(cdr L)))
          (else
            (findMax (cons (car L) (cddr L))))))

(define (depthOfList L)
    (cond
          ((null? L) 1)
          ((number? L) 0)

          (else (max (+ 1 (depthOfList (car L)))
            (depthOfList (cdr L))))))


(define (newlength L)
  (fold-left (lambda (sum element) (+ sum 1)) 0 L))

(define (addOne L)
  (if (null? L) '() (map (lambda (a) (+ a 1)) L)))

(define (lstOR L)
  (if (null? L) #f
    (if (null? (filter (lambda (x) (equal? x #t)) L)) #f #t)))

(define (removeDup L)
  (fold-left
   (lambda (a b)
    (cons b (filter (lambda (c)
     (not (= b c))) a)))
    '()
     (fold-left (lambda (i j) (cons j i)) '() L)))