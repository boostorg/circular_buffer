#!/bin/sh

DOCFILE="$1.html"

if [ -f $DOCFILE ]; then

  echo Starting Doxygen ...
  doxygen

  echo Generating source XHTML documentation ...
  xsltproc --stringparam container $1 --stringparam xmldir srcdoc -o srcdoc/srcdoc.xhtml $1.xslt srcdoc/index.xml

  echo Preprocessing original HTML documentation ...
  sed 's/<a\s*id="[^"]*"/<a /g' $DOCFILE | sed 's/<a\s*\(name="[^"]*"\)\s*\(id="[^"]*"\)/<a \1/g' > srcdoc/$DOCFILE

  echo Converting original HTML documentation into XHTML ...
  xsltproc --html -o srcdoc/$1.xhtml html2xhtml.xslt srcdoc/$DOCFILE

  echo Creating HTML documentation with updated source documentation ...
  xsltproc --stringparam srcdoc srcdoc/srcdoc.xhtml -o $DOCFILE update_srcdoc.xslt srcdoc/$1.xhtml

  echo Correcting and pretty-printing HTML documentation with HTML Tidy ...
  tidy -ashtml -config Tidy.conf $DOCFILE
  
  echo Removing temporary directory ...
  rm -rf srcdoc

  echo Done.

else

  echo Usage: update_srcdoc.sh container

fi
