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
  <head><title>GL Intercept version <xsl:apply-templates select="VERSION"/> Log generated on <xsl:apply-templates select="TIMESTAMP"/></title>
  
  <style type="text/css">

  #showimage{
  position:absolute;
  visibility:hidden;
  border: 1px solid gray;
  }

  #dragbar{
  cursor: hand;
  cursor: pointer;
  background-color: #EFEFEF;
  min-width: 100px; /*NS6 style to overcome bug*/
  }

  #dragbar #closetext{
  font-weight: bold;
  margin-right: 1px;
  }
  </style>

  <script type="text/javascript">

  /***********************************************
  * Image Thumbnail viewer- (c) Dynamic Drive (www.dynamicdrive.com)
  * Last updated Sept 26th, 03'. This notice must stay intact for use
  * Visit http://www.dynamicdrive.com/ for full source code
  ***********************************************/

  var ie=document.all
  var ns6=document.getElementById&amp;&amp;!document.all

  function ietruebody(){
  return (document.compatMode &amp;&amp; document.compatMode!="BackCompat" &amp;&amp; !window.opera)? document.documentElement : document.body
  }

  function enlarge(which, e, position, imgwidth, imgheight){
  if (ie||ns6){
  crossobj=document.getElementById? document.getElementById("showimage") : document.all.showimage
  if (position=="center"){
  pgyoffset=ns6? parseInt(pageYOffset) : parseInt(ietruebody().scrollTop)
  horzpos=ns6? pageXOffset+window.innerWidth/2-imgwidth/2 : ietruebody().scrollLeft+ietruebody().clientWidth/2-imgwidth/2
  vertpos=ns6? pgyoffset+window.innerHeight/2-imgheight/2 : pgyoffset+ietruebody().clientHeight/2-imgheight/2
  if (window.opera &amp;&amp; window.innerHeight) //compensate for Opera toolbar
  vertpos=pgyoffset+window.innerHeight/2-imgheight/2
  vertpos=Math.max(pgyoffset, vertpos)
  }
  else{
  var horzpos=ns6? pageXOffset+e.clientX : ietruebody().scrollLeft+event.clientX
  var vertpos=ns6? pageYOffset+e.clientY : ietruebody().scrollTop+event.clientY
  }
  crossobj.style.left=horzpos+"px"
  crossobj.style.top=vertpos+"px"

  crossobj.innerHTML='&lt;div align="left" id="dragbar"&gt;&lt;span id="closetext" onClick="closepreview()"&gt;Close&lt;/span&gt; &lt;/div&gt;&lt;img src="'+which+'"/&gt;'
  crossobj.style.visibility="visible"
  return false
  }
  else //if NOT IE 4+ or NS 6+, simply display image in full browser window
  return true
  }

  function closepreview(){
  crossobj.style.visibility="hidden"
  }

  function drag_drop(e){
  if (ie&amp;&amp;dragapproved){
  crossobj.style.left=tempx+event.clientX-offsetx+"px"
  crossobj.style.top=tempy+event.clientY-offsety+"px"
  }
  else if (ns6&amp;&amp;dragapproved){
  crossobj.style.left=tempx+e.clientX-offsetx+"px"
  crossobj.style.top=tempy+e.clientY-offsety+"px"
  }
  return false
  }

  function initializedrag(e){
  if (ie&amp;&amp;event.srcElement.id=="dragbar"||ns6&amp;&amp;e.target.id=="dragbar"){
  offsetx=ie? event.clientX : e.clientX
  offsety=ie? event.clientY : e.clientY

  tempx=parseInt(crossobj.style.left)
  tempy=parseInt(crossobj.style.top)

  dragapproved=true
  document.onmousemove=drag_drop
  }
  }

  document.onmousedown=initializedrag
  document.onmouseup=new Function("dragapproved=false")

  </script>  
  
  
  <script language="javascript" type="text/javascript">
  &lt;!--
  /****************************************************
       Author: Eric King
       Url: http://redrival.com/eak/index.shtml
       This script is free to use as long as this info is left in
       Featured on Dynamic Drive script library (http://www.dynamicdrive.com)
  ****************************************************/
  var win=null;
  function NewWindow(mypage,myname,w,h,scroll,pos){

  //Strip the extension from the name to keep ie happy
  myname = myname.substring(0,myname.indexOf("."));

  if(pos=="random"){LeftPosition=(screen.width)?Math.floor(Math.random()*(screen.width-w)):100;TopPosition=(screen.height)?Math.floor(Math.random()*((screen.height-h)-75)):100;}
  if(pos=="center"){LeftPosition=(screen.width)?(screen.width-w)/2:100;TopPosition=(screen.height)?(screen.height-h)/2:100;}
  else if((pos!="center" &amp;&amp; pos!="random") || pos==null){LeftPosition=0;TopPosition=20}
  
  //if ie5
  if (document.all&amp;&amp;window.print) 
  {
    settings='dialogWidth:'+w+'px;dialogHeight:'+h+'px;dialogTop:'+TopPosition+'px;dialogLeft:'+LeftPosition+'px;scroll:'+scroll+';resizable:yes;help:no';
    win=window.showModelessDialog(mypage,myname,settings);
  }
  else
  {
    settings='width='+w+',height='+h+',top='+TopPosition+',left='+LeftPosition+',scrollbars='+scroll+',location=no,directories=no,status=no,menubar=no,toolbar=no,resizable=yes,dependent=yes';
    win=window.open(mypage,myname,settings);
  }

  win.focus();
  }
  // --&gt;
  </script>
   
  </head>
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
  <body style="color: rgb(0, 0, 0); background-color: rgb(201, 199, 191);">
  <div id="showimage"/>


  <table cellpadding="2" cellspacing="2" border="0"
   style="text-align: left; width: 1264px; height: 112px;">
    <tbody>
      <tr>
        
        <td style="vertical-align: top;">
        <h1><span style="font-weight: bold;">GLIntercept log v<xsl:apply-templates select="/GLINTERCEPT/HEADER/VERSION"/></span></h1>
            Log generated on <xsl:apply-templates select="/GLINTERCEPT/HEADER/TIMESTAMP"/>
        </td>

        <td style="vertical-align: top; width: 30%;">
        <table cellpadding="2" cellspacing="0" border="1" style="text-align: left; width: 371px; height: 79px;">
          <tbody>
            <tr>
              <td style="vertical-align: top; background-color: rgb(170, 204, 136);">
              Do not conform any longer to the pattern of this world, but be transformed by the renewing of your mind.
              Then you will be able to test and approve what God's will is - his good, pleasing and perfect will. (Romans 12:2 NIV)
              </td>
            </tr>
          </tbody>
        </table>
        </td>
      </tr>
    </tbody>
  </table>
  <hr style="width: 100%; height: 2px;"/>

  <!-- <font size="2" face="Courier,Courier New" color="#000000">  -->
  <!-- <font color="#007f00">  -->

 <font size="+1" color="#007f00">
 <span style="font-family: fixedsys,Courier New,Courier;">
 <nobr>
  <xsl:apply-templates select="FUNCTION"/>
 </nobr>
 </span>
 </font>


  <!-- </font>  -->
  <!-- </font>  -->
  </body>
</xsl:template>

<!-- Main function template -->
<xsl:template name="MainFunction">
  <xsl:apply-templates select="NAME"/>(<xsl:choose>
   <xsl:when test="count(child::PARAM) > 0">
     <xsl:apply-templates select="PARAM"/>
   </xsl:when>

   <!-- If this function has no parameters- it is unknown -->
   <xsl:otherwise>???</xsl:otherwise>
  </xsl:choose>);

  <!-- Apply the return code -->
  <xsl:apply-templates select="RETURN"/>

  <!-- Apply the shaders -->
  <xsl:if test="count(child::SHADER) > 0">
  Shaders=<xsl:apply-templates select="SHADER"/>
  </xsl:if>

  <!-- Apply the images/textures -->
  <xsl:if test="count(child::TEXSTAGE) > 0">
  Tex=<xsl:apply-templates select="TEXSTAGE"/>
  </xsl:if>

  <!-- Apply the frame buffer images -->
  <xsl:apply-templates select="FRAMEBUFFER"/>

  <!-- Apply any error codes -->
  <xsl:apply-templates select="ERROR"/>

  <!-- Apply any optional text -->
  <xsl:apply-templates select="TEXT"/>

  <!-- Apply any optional debug info -->
  <xsl:apply-templates select="DEBUG"/>

  <!-- Apply any function timings -->
  <xsl:apply-templates select="FUNCTIME"/>

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
     <xsl:apply-templates select="VALUE"/>
   </xsl:when>

   <xsl:when test="count(child::VALUE) > 1">
     [<xsl:apply-templates select="VALUE"/>]
   </xsl:when>
 </xsl:choose>

 <xsl:apply-templates select="SHADER"/>
 <xsl:apply-templates select="IMAGE"/>
 <xsl:apply-templates select="DLIST"/>

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
<!-- Image processing -->
<!-- ========================================================================== -->

 <!-- Apply icons and images -->
<xsl:template match="IMAGE">

 <xsl:choose>
   <xsl:when test="@icon and @name">
      <a href="{@name}" onClick="return enlarge('{@name}',event)">
      <img src="{@icon}" border="0"/></a>
   </xsl:when>
   <xsl:when test="@name">
      <a href="{@name}" onClick="return enlarge('{@name}',event)">(img <xsl:value-of select="@id"/>)</a>
   </xsl:when>
   <xsl:when test="@icon">
      <img src="{@icon}" border="0"/>
   </xsl:when>
 </xsl:choose>

</xsl:template>

 <!-- Apply icons and images -->
<xsl:template match="TEXSTAGE/IMAGE[1]">
<xsl:value-of select="@id"/>

 <xsl:choose>
   <xsl:when test="@icon and @name">
      <a href="{@name}" onClick="return enlarge('{@name}',event)">
      <img src="{@icon}" border="0"/></a>
   </xsl:when>
   <xsl:when test="@name">
      <a href="{@name}" onClick="return enlarge('{@name}',event)">(img)</a>
   </xsl:when>
   <xsl:when test="@icon">
      <img src="{@icon}" border="0"/>
   </xsl:when>
 </xsl:choose>

</xsl:template>


<!-- ========================================================================== -->
<!-- Shader processing -->
<!-- ========================================================================== -->

<xsl:template match="PARAM/SHADER">

 <xsl:choose>
   <xsl:when test="@type and @name">
     <a href="{@name}" onclick="NewWindow(this.href,'{@name}','600','400','yes','center');return false" onfocus="this.blur()">(<xsl:value-of select="@type"/><xsl:value-of select="@id"/>)</a>
   </xsl:when>
   <xsl:when test="@name">
     <a href="{@name}" onclick="NewWindow(this.href,'{@name}','600','400','yes','center');return false" onfocus="this.blur()">(src<xsl:value-of select="@id"/>)</a>
   </xsl:when>
 </xsl:choose>

</xsl:template>


<xsl:template match="SHADER">

 <xsl:choose>
   <xsl:when test="@type and @name">
     <a href="{@name}" onclick="NewWindow(this.href,'{@name}','600','400','yes','center');return false" onfocus="this.blur()">(<xsl:value-of select="@type"/><xsl:value-of select="@id"/>)</a>
   </xsl:when>
   <xsl:when test="@name">
     <a href="{@name}" onclick="NewWindow(this.href,'{@name}','600','400','yes','center');return false" onfocus="this.blur()">(src<xsl:value-of select="@id"/>)</a>
   </xsl:when>
   <xsl:when test="@type">
     (<xsl:value-of select="@type"/><xsl:value-of select="@id"/>)
   </xsl:when>

   <xsl:otherwise>(src<xsl:value-of select="@id"/>)</xsl:otherwise>
 </xsl:choose>

</xsl:template>

<!-- ========================================================================== -->
<!-- Display List processing -->
<!-- ========================================================================== -->

<xsl:template match="PARAM/DLIST">

 <xsl:choose>
   <xsl:when test="@name">
     <a href="{@name}" onclick="NewWindow(this.href,'{@name}','600','400','yes','center');return false" onfocus="this.blur()">(src<xsl:value-of select="@id"/>)</a>
   </xsl:when>
 </xsl:choose>

</xsl:template>

<xsl:template match="DLIST">

 <xsl:choose>
   <xsl:when test="@name">
     <a href="{@name}" onclick="NewWindow(this.href,'{@name}','600','400','yes','center');return false" onfocus="this.blur()">(src<xsl:value-of select="@id"/>)</a>
   </xsl:when>

   <xsl:otherwise>(src<xsl:value-of select="@id"/>)</xsl:otherwise>
 </xsl:choose>

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
  <br/><b><font color="#FF0000"><xsl:apply-templates/></font></b>
</xsl:template>  

  
<!-- ========================================================================== -->
<!-- Function timings processing -->
<!-- ========================================================================== -->

<xsl:template match="FUNCTIME">
 Time = <xsl:value-of select="@data"/> us
</xsl:template>

<!-- ========================================================================== -->
<!-- Tex Stage processing -->
<!-- ========================================================================== -->

<xsl:template match="TEXSTAGE">(<xsl:value-of select="@number"/>,<xsl:apply-templates select="IMAGE"/>)
</xsl:template>


<!-- ========================================================================== -->
<!-- Frame buffer processing -->
<!-- ========================================================================== -->

<xsl:template match="FRAMEBUFFER">
<xsl:apply-templates select="COLORBUFFER"/>
<xsl:apply-templates select="DEPTHBUFFER"/>
<xsl:apply-templates select="STENCILBUFFER"/>
<xsl:apply-templates select="FRAMEMOVIE"/>
</xsl:template>

<xsl:template match="COLORBUFFER">
Color(<xsl:apply-templates select="PRE"/>
<xsl:apply-templates select="POST"/>
<xsl:apply-templates select="DIFF"/>)
</xsl:template>

<xsl:template match="DEPTHBUFFER">
Depth(<xsl:apply-templates select="PRE"/>
<xsl:apply-templates select="POST"/>
<xsl:apply-templates select="DIFF"/>)
</xsl:template>

<xsl:template match="STENCILBUFFER">
Stencil(<xsl:apply-templates select="PRE"/>
<xsl:apply-templates select="POST"/>
<xsl:apply-templates select="DIFF"/>)
</xsl:template>

<xsl:template match="PRE">
<a href="{@image}" onClick="return enlarge('{@image}',event)">
<xsl:choose>
   <xsl:when test="@icon">
      <img src="{@icon}" border="0"/>
   </xsl:when>
   <xsl:otherwise>Pre </xsl:otherwise>
</xsl:choose></a>
</xsl:template>

<xsl:template match="POST">
<a href="{@image}" onClick="return enlarge('{@image}',event)">
<xsl:choose>
   <xsl:when test="@icon">
      <img src="{@icon}" border="0"/>
   </xsl:when>
   <xsl:otherwise>Post </xsl:otherwise>
</xsl:choose></a>
</xsl:template>

<xsl:template match="DIFF">
<a href="{@image}" onClick="return enlarge('{@image}',event)">
<xsl:choose>
   <xsl:when test="@icon">
      <img src="{@icon}" border="0"/>
   </xsl:when>
   <xsl:otherwise>Diff </xsl:otherwise>
</xsl:choose></a>
</xsl:template>

<xsl:template match="FRAMEMOVIE">
Movie(<a href="{@video}">Frame</a>)
</xsl:template>


</xsl:stylesheet>



