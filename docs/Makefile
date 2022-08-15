.PHONY: public
public : 
	hugo -d ../public -b "https://pages.github.iu.edu/geobrown/NanoTag-paper/"

install : public
	rsync -rcv --delete --exclude=.git \
	../public/  ../../NanoTag-paper-gh-pages

.PHONY: clean
clean:
	rm -rf ../public

