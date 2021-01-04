; tests for p1
(LEFT (PAIR 3 4))  ; 3 
(RIGHT (PAIR 3 4))  ; 4
(LEFT (PAIR 3 (PAIR 4 5)))  ; 3
(RIGHT (RIGHT (PAIR 3 (PAIR 4 5))))  ; 5 

; tests for p2
((funcompose '(sqrt floor)) 9) ; 3
((funcompose '((lambda (x) (+ 1 x)) floor)) 3.2) ; 4.0
((funcompose '((lambda (x) (> 1 x)) sqrt)) 4) ; #f

; tests for p3
(merge '(2 7) '()) ; returns (2 7)
(merge '(1 1 2) '(1 3 5)) ; returns (1 1 1 2 3 5)
(merge '(1 1 6 8) '(2 7)) ; returns (1 1 2 6 7 8)

; tests for p4
(findMax  '(4 5 1))      ; returns 5
(findMax  '(5 1 (3 (4 8)))) ; returns 8
(findMax  '(1 3 (3 3) () 6 (1))) ; returns 6

; tests for p5
(depthOfList '()) ; returns 1
(depthOfList '(1 2 (1 (2 3)))) ; returns 3
(depthOfList '(0 (0 ()) ())) ; returns 3


; tests for p6
(newlength '()) ; 0
(newlength '(1 1 2)) ; 3
(newlength '(3 (5 2) (7 (7) 7) 9)) ; 4

(addOne '(1 2 3 4)) ; (2 3 4 5)
(addOne '(2 4 6 8)) ; (3 5 7 9)
(addOne '()) ; ()

(lstOR '(#t #f)) ; #t
(lstOR '(#f #f)) ; #f
(lstOR '()) ; #f

(removeDup '(1 1 2)) ; (1 2)
(removeDup '(3 5 5 7 7 7 9)) ; (3 5 7 9)
(removeDup '()) ; ()
