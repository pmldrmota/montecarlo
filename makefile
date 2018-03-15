all: test.exe test_archive.exe bayes_inference.exe volume_sphere.exe pi.exe integrate.exe bimodal_walk.exe

test.exe: source/test.cpp archives.o mc.o rd.o mcmc.o inference.o metropolis.o
	g++ -std=c++11 -iquote source/ -O $^ -o $@

test_archive.exe: source/archives/test_archive.cpp archives.o mc.o rd.o mcmc.o inference.o metropolis.o
	g++ -std=c++11 -iquote source/ -O $^ -o $@

bayes_inference.exe: source/inference/bayes_inference.cpp archives.o mc.o mcmc.o inference.o
	g++ -std=c++11 -iquote source/ -O $^ -o $@

bimodal_walk.exe: source/metropolis/bimodal_walk.cpp archives.o mc.o mcmc.o metropolis.o
	g++ -std=c++11 -iquote source/ -O  $^ -o $@

volume_sphere.exe: source/volume_sphere.cpp archives.o mc.o mcmc.o metropolis.o rd.o
	g++ -std=c++11 -iquote source/ -O  $^ -o $@

pi.exe: source/rd/pi.cpp mc.o archives.o rd.o
	g++ -std=c++11 -iquote source/ -O  $^ -o $@

integrate.exe: source/rd/integrate.cpp archives.o mc.o rd.o
	g++ -std=c++11 -iquote source/ -O  $^ -o $@

mc.o: source/common/mc.cpp
	g++ -std=c++11 -iquote source/ -O -c $<

archives.o: source/archives/archives.cpp
	g++ -std=c++11 -iquote source/ -O -c $<

rd.o: source/rd/rd.cpp
	g++ -std=c++11 -iquote source/ -O -c $<

mcmc.o: source/common/mcmc.cpp
	g++ -std=c++11 -iquote source/ -O -c $<

inference.o: source/inference/inference.cpp
	g++ -std=c++11 -iquote source/ -O -c $<

metropolis.o: source/metropolis/metropolis.cpp
	g++ -std=c++11 -iquote source/ -O -c $<