.PHONY: all traitement inference main clean veryclean

all: traitement inference main

traitement:
	$(MAKE) -C Traitement

inference:
	$(MAKE) -C Inference

main:
	gcc main.c -o launcher

clean:
	$(MAKE) -C Traitement clean
	$(MAKE) -C Inference clean
	rm -f launcher

veryclean:
	$(MAKE) -C Traitement veryclean
	$(MAKE) -C Inference veryclean
	rm -f launcher *~ *.o