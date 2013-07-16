#compiling...
gwb:	*.h *.cpp
	g++ *.h *.cpp -o gwb -LGL -lglut -lglui
#eg0 hemisphere
eg eg0 facehemi:	gwb	
	./gwb face hemisphere 6 20 20
#eg1 sphere
eg1 sphere wiresphere:	gwb	
	./gwb wire sphere 6 20 20
#eg2 torus
eg2 donut torus wiretorus:	gwb	
	./gwb wire torus 6 4 20 20
#eg3 facecylinder
eg3 facecylinder:	gwb	
	./gwb face cylinder 6 20 20 20
#eg4 wirecylinder
eg4 cylinder wirecylinder:	gwb	
	./gwb wire cylinder 6 20 20 20
#eg5 disk 
eg5 disk wiredisk:	gwb	
	./gwb wire disk 10 5 20 20
#eg6 facedisk 
eg6 facedisk:	gwb	
	./gwb face disk 10 5 20 20
