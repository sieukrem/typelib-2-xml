<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" encoding="utf-8" doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"  doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"/>
 
<xsl:template match="/">
 <html><head>
 <title><xsl:value-of select="TypeLibrary/@name" /> - API</title>
 <link rel="stylesheet" type="text/css" href="static/typelib2xml.css"/>
 <meta http-equiv="content-type" content="text/html; charset=utf-8" />

 </head><body>
	<xsl:apply-templates select="TypeLibrary"/>
	
 </body>
 </html>
</xsl:template>

<xsl:template match="TypeLibrary">

	<h1><xsl:value-of select="@name" /></h1>
	<div>
		<xsl:apply-templates select="documentation" />
	</div>
	<xsl:apply-templates select="type" />
	
</xsl:template>

<xsl:template match="type">
	<div class="type" id="Type-{@name}">
		<h2><xsl:value-of select="@name" /><a class="anchor" href="#Type-{@name}"/></h2>
		<div class="documentation">
			<xsl:apply-templates select="documentation" />
		</div>
		
		<xsl:if test="impl">
			<div class="implements">
				<b>Implements: </b><xsl:apply-templates select="impl"/>
			</div>
		</xsl:if>

		<xsl:apply-templates select=".[function]" mode="table" />
	</div>
</xsl:template>

<xsl:template match="type[@kind='enum']" mode="table">
	<table class='enum'>
		<tr>
			<th></th>
			<th>Name</th>
			<th>Value</th>
			<th>Description</th>
		</tr>
		<xsl:apply-templates select="variable">
			<xsl:sort select="@name"></xsl:sort>
		</xsl:apply-templates>
	</table>

</xsl:template>

<xsl:template match="type" mode="table">
	<table>
		<tr>
			<th></th>
			<th>Name</th>
			<th>Type</th>
			<th>Description</th>
		</tr>
		<xsl:apply-templates select="function">
			<xsl:sort select="@name"></xsl:sort>
		</xsl:apply-templates>
	</table>

</xsl:template>

<xsl:template match="variable">
	<tr>
		<td><span class="img {@kind}"> </span></td>
		<td><xsl:value-of select="@name" /></td>
		<td><xsl:value-of select="@value"/></td>
		<td><xsl:apply-templates select="documentation" /></td>
	</tr>
</xsl:template>

<xsl:template match="function[@invoke_kind='propertyget' or @invoke_kind='propertyput' or @invoke_kind='propertyputref']">
	<xsl:variable name="same-name" select="../function[@name=current()/@name]"></xsl:variable>
	<xsl:if test="generate-id($same-name[1])=generate-id()">
		<tr class="{generate-id()} {generate-id($same-name[1])}">
			<td><span class="img {@invoke_kind}"> </span></td>
			<td><xsl:value-of select="@name" /></td>
			<td><xsl:apply-templates select="param" mode="param-types" /></td>
			<td>
				<xsl:apply-templates select="$same-name/@invoke_kind"/>
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="documentation" />
			</td>
		</tr>
	</xsl:if>
</xsl:template>

<xsl:template match="@invoke_kind[.='propertyget']">
	<xsl:text>Gets</xsl:text>
	<xsl:if test="position()!=last()"> or </xsl:if>
</xsl:template>

<xsl:template match="@invoke_kind[.='propertyput']">
	<xsl:text>Sets</xsl:text>
	<xsl:if test="position()!=last()"> or </xsl:if>
</xsl:template>

<xsl:template match="function">
	<xsl:variable name="output" select="param[out]" />
	
	<xsl:variable name="params" select="param[not(retval) and count($output)&gt;1]|param[2&gt;count($output) and not(out)]" />
	
	<xsl:variable name="retval" select="param[retval]|param[1=count($output) and out]" />

	<tr>
		<td><span class="img {@invoke_kind}"> </span></td>
		<td>
			<div>
				<xsl:value-of select="@name" />
				<xsl:if test="$params">(</xsl:if>
				<xsl:if test="not($params)">()</xsl:if>
			</div>

			<div class="params">
				<xsl:apply-templates select="$params" mode="param-names" />
			</div>
			
			<xsl:if test="$params"><div>)</div></xsl:if>
		</td>
		
		<td class="types">
			<xsl:if test="$params">
				<div class="empty"></div>
			</xsl:if>
			
			<xsl:apply-templates select="$params" mode="param-types"/>
						
			<xsl:if test="$params and not($retval)">
				<div class="empty"></div>
			</xsl:if>	
			
			<xsl:if test="$retval">
				<xsl:apply-templates select="$retval" mode="param-types" />
			</xsl:if>
		</td>
		
		<td><xsl:apply-templates select="documentation" /></td>
	</tr>
</xsl:template>


<xsl:template match="param" mode="param-names">
	<div class="param"><xsl:value-of select="@name" /></div>
</xsl:template>

<xsl:template match="param" mode="param-types">
	<div><xsl:apply-templates select="@typedesc"/></div>
</xsl:template>

<xsl:template match="documentation|@documentation">
	<xsl:copy-of select="." />
</xsl:template>

<xsl:template match="@documentation">
	<xsl:value-of select="." />
</xsl:template>

<xsl:template match="function" mode="type">
	<xsl:variable name="type-name" select="param[out|in]/@typedesc"></xsl:variable>
	<xsl:apply-templates select="param[out|in]/@typedesc"/>
</xsl:template>

<xsl:template match="@typedesc">
	<xsl:choose>
		<xsl:when test="//type[@name = current()]">
			<a href="#Type-{current()}"><xsl:value-of select="current()" /></a>
		</xsl:when>
		<xsl:otherwise>
			<xsl:value-of select="current()" />
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match="impl">

	<xsl:choose>
		<xsl:when test="//type[@name = current()/@name]">
			<a href="#Type-{@name}"><xsl:value-of select="@name" /></a>
		</xsl:when>
		<xsl:otherwise>
			<xsl:value-of select="@name" />
		</xsl:otherwise>
	</xsl:choose>
	
	<xsl:if test="position() !=1">, </xsl:if>
</xsl:template>

</xsl:stylesheet>
