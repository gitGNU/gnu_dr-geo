
(nouvelle-figure "MON CARRE")

(define cote 3)

(define (monCarre origin)
  (let* ((O1 (Point "O1" libre 0 0))
	 (E1 (Point "E1" libre 
		    (/ cote (sqrt 2)) 
		    (/ cote (sqrt 2))))
	 (E2 (Point "E2" libre 
		    (/ (- 0 cote) (sqrt 2)) 
		    (/ cote (sqrt 2))))
	 (v1 (Vecteur "v1" 2points O1 E1))
	 (v2 (Vecteur "v2" 2points O1 E2))
	 (A  (Point "" translation origin v1))
	 (B (Point "" translation origin v2))
	 (s1 (Segment "" extrémités origin A))
	 (s2 (Segment "" extrémités origin B)))
    
    (Segment "" translation s1 v2)
    (Segment "" translation s2 v1)
    
    (send O1 masquer)
    (send E1 masquer)
    (send E2 masquer)
    (send v1 masquer)
    (send v2 masquer)
    (send A masquer)
    (send B masquer)))

(soit Point "O" libre -1 -1)

(monCarre O)

