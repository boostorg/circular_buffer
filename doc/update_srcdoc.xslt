<?xml version='1.0' encoding='utf-8'?>
<!--
XSL transformation updating source code documentation section in the specified file.
Author: Jan Gaspar (jano_gaspar[at]yahoo.com)
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
    <xsl:output method="html" version="1.0" encoding="iso-8859-1" indent="yes" omit-xml-declaration="yes" media-type="text/html"/>

    <xsl:param name="srcdoc"/>

    <xsl:template match="*">
        <xsl:copy>
            <xsl:apply-templates select="*|@*|text()|comment()|processing-instruction()"/>
        </xsl:copy>
    </xsl:template>

    <xsl:template match="@*|text()|comment()|processing-instruction()">
        <xsl:copy-of select="."/>
    </xsl:template>

    <xsl:template match="div[@id='srcdoc']">
        <div id="srcdoc">
            <xsl:copy-of select="document($srcdoc)//body/*"/>
        </div>
    </xsl:template>

</xsl:stylesheet>
