projet : affichage.o autotests.o fichiers.o fir.o iir.o mesure.o main_sim.o
	gcc $^ -o $@

%.o : %.c
	gcc -c $< -o $@

test : affichage.o autotests.o fichiers.o fir.o iir.o mesure.o main_test.o
	gcc $^ -o $@
	
clean :
	rm *.o projet