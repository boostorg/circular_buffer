#!/bin/sh

DOCFILE="$1.html"

if [ -f $DOCFILE ]; then

  echo Starting Doxygen ...
  doxygen

  echo Generating source XHTML documentation ...
  xsltproc --stringparam container $1 --stringparam xmldir srcdoc -o srcdoc/srcdoc.xhtml $1.xslt srcdoc/index.xml

  echo Converting original HTML documentation into XHTML ...
  tidy --doctype omit -asxhtml -o srcdoc/$1.xhtml -config Tidy.conf $DOCFILE

  echo Creating HTML documentation with updated source documentation ...
  xsltproc --novalid --stringparam srcdoc srcdoc/srcdoc.xhtml -o srcdoc/$DOCFILE update_srcdoc.xslt srcdoc/$1.xhtml

  echo Correcting and pretty-printing HTML documentation with HTML Tidy ...
  tidy -ashtml -o $DOCFILE -config Tidy.conf srcdoc/$DOCFILE

  echo Removing temporary directory ...
  rm -rf srcdoc

  echo Done.

else

  echo Usage: update_srcdoc.sh container

fi
