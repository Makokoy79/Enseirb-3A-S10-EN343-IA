.PHONY: all traitement inference main clean veryclean

all: traitement inference main

traitement:
	$(MAKE) -C Traitement

inference:
	$(MAKE) -C Inference

main:
	gcc main.c -o all

clean:
	$(MAKE) -C Traitement clean
	$(MAKE) -C Inference clean
	rm -f all

veryclean:
	$(MAKE) -C Traitement veryclean
	$(MAKE) -C Inference veryclean
	rm -f all *~ *.o