(new-figure "Flocon de Koch")
(lets Numeric "a1" free 1 2 (acos 0.5))
(lets Numeric "k1" free 0 0 (/ 1 3))
(lets Numeric "k2" free 0 1 (/ 2 3))
(send a1 masked)
(send k1 masked)
(send k2 masked) 
(define (koch p1 p2 n)

(let* (  
       
    (p3 (Point "" homothétie p2 p1 k1))
    (p4 (Point "" homothétie p2 p1 k2))
    (p5 (Point "" rotation p4 p3 a1))

    (s1 (Segment "" extremities p1 p3))
    (s2 (Segment "" extremities p4 p2))
    (s3 (Segment "" extremities p3 p5))
    (s4 (Segment "" extremities p5 p4)))  
 
   
    (if (> n 1)
    (begin
    (send s1 masked)
    (send s2 masked)
    (send s3 masked)
    (send s4 masked)))    
   
    (send p1 masked)
    (send p2 masked)
    (send p3 masked)
    (send p5 masked)
    (send p4 masked)
     
    (if (> n 0)
    (begin 
     (koch p1 p3 (- n 1))
     (koch p3 p5 (- n 1))
     (koch p5 p4 (- n 1))
     (koch p4 p2 (- n 1))))))

(lets Point "A" free -5 0)
(lets  Point "B" free 5 0)
(lets Point "C" rotation A B a1)
(send A masked)
(send B masked)
(send C masked)

(koch A B 6)
(koch C A 6)
(koch B C 6)