#!/bin/sh

DOCFILE="$1.html"

if [ -f $DOCFILE ]; then

  echo Starting Doxygen ...
  doxygen

  echo Generating source XHTML documentation ...
  xsltproc --stringparam container $1 --stringparam xmldir srcdoc -o srcdoc/srcdoc.xhtml $1.xslt srcdoc/index.xml

  echo Converting original HTML documentation into XHTML ...
  xsltproc --html -o srcdoc/$1.xhtml html2xhtml.xslt $DOCFILE

  echo Creating HTML documentation with updated source documentation ...
  xsltproc --stringparam srcdoc srcdoc/srcdoc.xhtml -o $DOCFILE update_srcdoc.xslt srcdoc/$1.xhtml

  echo Removing temporary directory ...
  rm -rf srcdoc

  echo Done.

else

  echo Usage: update_srcdoc.sh container

fi
