FLAGS := -Wall -lm 

all: graph histogram scatter

scatter: scatter_main.c list.o scatter.o plot_options.o plot.o
	gcc $^ -o $@ $(FLAGS)

graph: graph_main.c list.o expression.o plot_options.o plot.o graph.o 
	gcc $^ -o $@ $(FLAGS)

histogram: histogram_main.c histogram.o list.o plot_options.o plot.o hist_options.o 
	gcc $^ -o $@ $(FLAGS)

histogram.o: histogram.c histogram.h
	gcc -c $< $(FLAGS)

hist_options.o: hist_options.c hist_options.h
	gcc -c $< $(FLAGS)

list.o: list.c list.h
	gcc -c $< $(FLAGS) 

expression.o: expression.c expression.h
	gcc -c $< $(FLAGS) 

plot.o: plot.c plot.h 
	gcc -c $< $(FLAGS) 

plot_options.o: plot_options.c plot_options.h 
	gcc -c $< $(FLAGS) 

graph.o: graph.c graph.h
	gcc -c $< $(FLAGS) 

scatter.o: scatter.c scatter.h
	gcc -c $< $(FLAGS) 
