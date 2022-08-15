SVGDIR = ../static/generated

%.pdf: %.tex
	pdflatex $<

$(SVGDIR)/%.svg: %.pdf
	pdf2svg $< $@

all: $(SVGDIR)/lifecycle.svg