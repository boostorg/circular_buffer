<?xml version='1.0' encoding='utf-8'?>
<!--
Helper XSL transformation which converts HTML into XHTML.
Author: Jan Gaspar (jano_gaspar[at]yahoo.com)
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="xml" version="1.0" encoding="utf-8" indent="yes" omit-xml-declaration="yes" media-type="text/html"/>

    <xsl:template match="*">
        <xsl:copy-of select="."/>
    </xsl:template>
    
</xsl:stylesheet>
