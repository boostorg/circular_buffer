<?xml version='1.0' encoding='iso-8859-1'?>

<!--
XSL transformation updating source code documentation sections in the specified file.
Author: Jan Gaspar (jano_gaspar[at]yahoo.com)
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:import href="copy.xslt"/>

  <xsl:output method="html" version="1.0" encoding="iso-8859-1" indent="yes" omit-xml-declaration="yes" media-type="text/html"/>

  <xsl:param name="srcdoc"/>

  <xsl:template match="div[@id='srcdoc_synopsis']">
    <xsl:copy-of select="document($srcdoc)//body/div[@id=current()/@id]"/>
  </xsl:template>

  <xsl:template match="div[@id='srcdoc_types']">
    <xsl:copy-of select="document($srcdoc)//body/div[@id=current()/@id]"/>
  </xsl:template>

  <xsl:template match="div[@id='srcdoc_constructors']">
    <xsl:copy-of select="document($srcdoc)//body/div[@id=current()/@id]"/>
  </xsl:template>

  <xsl:template match="div[@id='srcdoc_methods']">
    <xsl:copy-of select="document($srcdoc)//body/div[@id=current()/@id]"/>
  </xsl:template>

  <xsl:template match="div[@id='srcdoc_functions']">
    <xsl:copy-of select="document($srcdoc)//body/div[@id=current()/@id]"/>
  </xsl:template>

</xsl:stylesheet>
