<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<!-- ========================================================================== -->
<!-- Root info -->
<!-- ========================================================================== -->

<xsl:template match="GLINTERCEPT">
  <html>
  <xsl:apply-templates select="HEADER"/>
  <xsl:apply-templates select="FUNCTIONS"/>
  </html>
</xsl:template>

<!-- ========================================================================== -->
<!-- Header info -->
<!-- ========================================================================== -->

<xsl:template match="HEADER">
  <head><title>GL Intercept version <xsl:apply-templates select="VERSION"/> Run on <xsl:apply-templates select="TIMESTAMP"/></title></head>
</xsl:template>

<xsl:template match="VERSION">
 <xsl:apply-templates/>
</xsl:template>

<xsl:template match="TIMESTAMP">
 <xsl:apply-templates/>
</xsl:template>

<!-- ========================================================================== -->
<!-- Function info -->
<!-- ========================================================================== -->
<xsl:template match="FUNCTIONS">
  <body><xsl:apply-templates select="FUNCTION"/></body>
</xsl:template>

<!-- Main function template -->
<xsl:template name="MainFunction">
  <xsl:apply-templates select="NAME"/>

  <!-- If this function has no parameters- it is unknown -->
  (<xsl:choose>
   <xsl:when test="count(child::PARAM) > 0">
     <xsl:apply-templates select="PARAM"/>
   </xsl:when>

   <xsl:otherwise>???</xsl:otherwise>
  </xsl:choose>)

  <!-- Apply the return code -->
  <xsl:apply-templates select="RETURN"/>

  <!-- Apply any error codes -->
  <xsl:apply-templates select="ERROR"/>

  <!-- Apply any optional text -->
  <xsl:apply-templates select="TEXT"/>

  <!-- Apply any optional debug info -->
  <xsl:apply-templates select="DEBUG"/>

  <!-- Recursive call -->
  <xsl:apply-templates select="FUNCTION"/>
</xsl:template>

 <!-- Single function -->
<xsl:template match="FUNCTIONS/FUNCTION">
  <br/><xsl:call-template name="MainFunction"/>
</xsl:template>

<!-- Function called from a function -->
<xsl:template match="FUNCTION/FUNCTION">
  <br/><b><xsl:text>-----> </xsl:text></b><xsl:call-template name="MainFunction"/>
</xsl:template>

<xsl:template match="NAME">
 <xsl:apply-templates/>
</xsl:template>

<!-- ========================================================================== -->
<!-- Parameter processing -->
<!-- ========================================================================== -->

<xsl:template match="PARAM">
 
 <!-- Choose between an array of values and 1 value -->
 <xsl:choose>
   <xsl:when test="count(child::VALUE) = 1">
     <xsl:apply-templates/>
   </xsl:when>

   <xsl:when test="count(child::VALUE) > 1">
     [<xsl:apply-templates/>]
   </xsl:when>
 </xsl:choose>

 <!-- Add a comma -->
 <xsl:if test="position() != last()">,</xsl:if>

</xsl:template>

<xsl:template match="VALUE">

 <!-- Select the data -->
 <xsl:value-of select="@data"/>

 <!-- Add a comma -->
 <xsl:if test="position() != last()">,</xsl:if>
</xsl:template>


<!-- ========================================================================== -->
<!-- Return processing -->
<!-- ========================================================================== -->

<xsl:template match="RETURN">
 = <xsl:apply-templates select="VALUE"/> 
</xsl:template>


<!-- ========================================================================== -->
<!-- Error processing -->
<!-- ========================================================================== -->

<xsl:template match="ERROR">
 glGetError() = <xsl:apply-templates select="VALUE"/> 
</xsl:template>


<!-- ========================================================================== -->
<!-- Text processing -->
<!-- ========================================================================== -->

<xsl:template match="TEXT">
  <xsl:apply-templates/>
</xsl:template>


<!-- ========================================================================== -->
<!-- Debug processing -->
<!-- ========================================================================== -->

<xsl:template match="DEBUG">
  <br/><xsl:apply-templates/>
</xsl:template>


</xsl:stylesheet>
