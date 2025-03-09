# TYPELIB-2-XML

`TYPELIB-2-XML` exports typelib information embedded in a `.dll` or `.exe` as `xml` for further transformations e.g. `XSLT`.

```sh
typelib-2-xml.exe jn.dll api.xml
```

The [static](static) folder contains `typelib2xml.xsl` file which transforms typelib xml into a HTML page.