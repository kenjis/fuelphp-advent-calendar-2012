#!/bin/sh

# create ReVIEW files from Markdown files
sh make-re.sh

# create PDF
cd `dirname $0`
#review-preproc --tabwidth=2 --replace *.re
rm -f fuelphp-advent-calendar-2012.pdf
review-pdfmaker fuelphp-advent-calendar-2012.yaml

