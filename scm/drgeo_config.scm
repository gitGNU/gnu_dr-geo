;;
;;  Dr Geo
;; (C) Copyright OFSET 2003
;; Author: hilaire@ofset.org 
;; 
;;
;;
;;
;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public Licences as by published
;; by the Free Software Foundation; either version 2; or (at your option)
;; any later version
;;
;; This program is distributed in the hope that it will entertaining,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of 
;; MERCHANTABILTY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
;; Publis License for more details.
;;
;; You should have received a copy of the GNU General Public License along
;; with this program; if not, write to the Free Software Foundation, Inc.
;; 675 Mass Ave, Cambridge, MA 02139, USA.
;;

;;
;; Load user configuration
;;
(define (drgeoConfigLoad filename)
  (let ((port 0)
        (config '()))
    (set! port (open-input-file filename))
    (set! config (read port))
    (close-input-port port)  
    config))

;;
;; Load & Update use configuration file
;;
(define (drgeoConfigLoadAndUpdate defaultFilename userFilename)
  (define defaultConf (drgeoConfigLoad defaultFilename))
  (define userConf (drgeoConfigLoad userFilename))
  (updateUserConf userConf defaultConf))

;;
;; test if attr can be found somewhere in conf and return it or null
;;
(define (foundAttribute? attr conf)
  (cond ((null? conf) #f)
        ((eq? (caar conf) attr) #t)
        (else (foundAttribute? attr (cdr conf)))))
  
;;
;; update user conf. with defaut conf.
;;
(define (updateUserConf userConf defaultConf)
  (cond ((null? defaultConf) userConf)
        ((foundAttribute? (caar defaultConf) userConf)      
         (updateUserConf userConf (cdr defaultConf)))
        (else (updateUserConf (append userConf (list (car defaultConf))) 
                              (cdr defaultConf)))))

;;
;; Save user configuration
;;
(define (drgeoConfigSave filename myConf)
  (let ((port 0))
    (delete-file filename)
    (set! port (open-output-file filename))
    (write myConf port)
    (close-output-port port)))

;;
;; Set the prefered value for attr
;;
(define (drgeoConfigSet attr value myConf)
  (cond ((null? myConf) (append myConf (list (list attr value))))
         ((eq? (caar myConf) attr) (append (list (list attr value)) (cdr myConf)))
         (else (append (list (car myConf)) (drgeoConfigSet attr value (cdr myConf))))))

;;
;; Get the prefered value for attr
;;
(define (drgeoConfigGet attr myConf)
         (cond ((null? myConf) 
                (display "Cannot find attribue name ") 
                (display attr) (newline))
               ((eq? (caar myConf) attr) (cadar myConf))
               (else (drgeoConfigGet attr (cdr myConf)))))
;;

