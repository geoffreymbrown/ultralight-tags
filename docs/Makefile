.PHONY: public
public : 
	hugo -d ../public -b "https://pages.github.com/geoffreymbrown/abt-tag"

install : public
	rsync -rcv --delete --exclude=.git \
	../public/  ../../abt-tag-gh-pages

.PHONY: clean
clean:
	rm -rf ../public

