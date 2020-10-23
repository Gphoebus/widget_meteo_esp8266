
function IsIE() {
    var userAgent = navigator.userAgent.toLowerCase();
    if ((document.all && userAgent.indexOf('msie') != -1) || userAgent.indexOf('trident') != -1) {
        return true;
    }
    else {
        return false;
    }
}


function EmbedVideoObject(filePath, width, height) {

    document.write("<OBJECT ");
    
	if (width > 0 && height > 0) {
	    document.write( "width=\"" + width + "\" height=\"" + height + "\" ");
    }

	if (IsIE()) {
	    document.write( 'classid="CLSID:6BF52A52-394A-11d3-B153-00C04F79FAA6" ');
	} else {
	    document.write( 'type="application/x-ms-wmp" ');
	}
    document.write( " data=\"" + filePath + "\">");

//	document.write( '<PARAM NAME="uiMode" VALUE="' + uimode + '"> '
	document.write( '<PARAM NAME="autostart" VALUE=\"false\""> ');
	document.write( '<PARAM NAME="URL" VALUE="' + filePath + '"> </OBJECT> ');

};


function EmbedFlashObject(filePath, width, height) {

	document.write("<p><object classid=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\"\n");
	document.write("codebase=\"http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,79,0\"\n");
	document.write("width=\"" + width + "\" height=\"" + height + "\" >\n");
    document.write("<param name=movie value=\"" + filePath + "\">\n");
    document.write("<param name=quality value=high>\n");
	document.write("<EMBED SRC=\"" + filePath + "\" WIDTH=\"" + width + "\" HEIGHT=\"" + height + "\" quality=\"high\"  NAME=\"movie\" TYPE=\"application/x-shockwave-flash\" PLUGINSPAGE=\"http://www.macromedia.com/go/getflashplayer\">\n");
	document.write("</object></p>\n");

};
