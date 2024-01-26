/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MediaTypes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 22:15:35 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/26 12:11:59 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/MediaTypes.hpp"

std::string getMediaType( std::string &file )
{
	static std::map<std::string, std::string> media_type;

	if ( ! media_type.empty())
	{
		size_t pos = file.find_last_of(".");
		if ( pos == std::string::npos )
			return "application/octet-stream";
		std::string extension = file.substr(pos);
		if ( media_type.find(extension) != media_type.end() )
			return media_type[extension];
		return "application/octet-stream";
	}

	media_type[".323"] = "text/h323";
	media_type[".3gp"] = "video/3gpp";
	media_type[".3g2"] = "video/3gpp2";
	media_type[".3gpp"] = "video/3gpp";
	media_type[".3gp2"] = "video/3gpp2";
	media_type[".aa"] = "audio/audible";
	media_type[".7z"] = "application/x-7z-compressed";
	media_type[".AAC"] = "audio/aac";
	media_type[".aax"] = "audio/vnd.audible.aax";
	media_type[".aaf"] = "application/octet-stream";
	media_type[".ac3"] = "audio/ac3";
	media_type[".aca"] = "application/octet-stream";
	media_type[".accda"] = "application/msaccess.addin";
	media_type[".accdc"] = "application/msaccess.cab";
	media_type[".accdb"] = "application/msaccess";
	media_type[".accde"] = "application/msaccess";
	media_type[".accdt"] = "application/msaccess";
	media_type[".accdr"] = "application/msaccess.runtime";
	media_type[".accdw"] = "application/msaccess.webapplication";
	media_type[".acx"] = "application/internet-property-stream";
	media_type[".accft"] = "application/msaccess.ftemplate";
	media_type[".AddIn"] = "text/xml";
	media_type[".adobebridge"] = "application/x-bridge-url";
	media_type[".ade"] = "application/msaccess";
	media_type[".adp"] = "application/msaccess";
	media_type[".ADTS"] = "audio/aac";
	media_type[".ADT"] = "audio/vnd.dlna.adts";
	media_type[".ai"] = "application/postscript";
	media_type[".afm"] = "application/octet-stream";
	media_type[".aif"] = "audio/aiff";
	media_type[".aiff"] = "audio/aiff";
	media_type[".aifc"] = "audio/aiff";
	media_type[".air"] = "application/vnd.adobe.air-application-installer-package+zip";
	media_type[".anx"] = "application/annodex";
	media_type[".amc"] = "application/mpeg";
	media_type[".apk"] = "application/vnd.android.package-archive";
	media_type[".application"] = "application/x-ms-application";
	media_type[".apng"] = "image/apng";
	media_type[".art"] = "image/x-jg";
	media_type[".asa"] = "application/xml";
	media_type[".asax"] = "application/xml";
	media_type[".ascx"] = "application/xml";
	media_type[".asd"] = "application/octet-stream";
	media_type[".asf"] = "video/x-ms-asf";
	media_type[".asi"] = "application/octet-stream";
	media_type[".ashx"] = "application/xml";
	media_type[".asm"] = "text/plain";
	media_type[".aspx"] = "application/xml";
	media_type[".asmx"] = "application/xml";
	media_type[".asr"] = "video/x-ms-asf";
	media_type[".asx"] = "video/x-ms-asf";
	media_type[".atom"] = "application/atom+xml";
	media_type[".avci"] = "image/avci";
	media_type[".au"] = "audio/basic";
	media_type[".avcs"] = "image/avcs";
	media_type[".avi"] = "video/x-msvideo";
	media_type[".avif"] = "image/avif";
	media_type[".axa"] = "audio/annodex";
	media_type[".avifs"] = "image/avif-sequence";
	media_type[".axs"] = "application/olescript";
	media_type[".axv"] = "video/annodex";
	media_type[".bas"] = "text/plain";
	media_type[".bin"] = "application/octet-stream";
	media_type[".bcpio"] = "application/x-bcpio";
	media_type[".bmp"] = "image/bmp";
	media_type[".c"] = "text/plain";
	media_type[".caf"] = "audio/x-caf";
	media_type[".cab"] = "application/octet-stream";
	media_type[".calx"] = "application/vnd.ms-office.calx";
	media_type[".cat"] = "application/vnd.ms-pki.seccat";
	media_type[".cc"] = "text/plain";
	media_type[".cdda"] = "audio/aiff";
	media_type[".cd"] = "text/plain";
	media_type[".cdf"] = "application/x-cdf";
	media_type[".cer"] = "application/x-x509-ca-cert";
	media_type[".cfg"] = "text/plain";
	media_type[".class"] = "application/x-java-applet";
	media_type[".chm"] = "application/octet-stream";
	media_type[".clp"] = "application/x-msclip";
	media_type[".cmd"] = "text/plain";
	media_type[".cmx"] = "image/x-cmx";
	media_type[".cod"] = "image/cis-cod";
	media_type[".cnf"] = "text/plain";
	media_type[".config"] = "application/xml";
	media_type[".contact"] = "text/x-ms-contact";
	media_type[".cpio"] = "application/x-cpio";
	media_type[".coverage"] = "application/xml";
	media_type[".cpp"] = "text/plain";
	media_type[".crl"] = "application/pkix-crl";
	media_type[".crd"] = "application/x-mscardfile";
	media_type[".crt"] = "application/x-x509-ca-cert";
	media_type[".csdproj"] = "text/plain";
	media_type[".cs"] = "text/plain";
	media_type[".csh"] = "application/x-csh";
	media_type[".css"] = "text/css";
	media_type[".csproj"] = "text/plain";
	media_type[".csv"] = "text/csv";
	media_type[".czx"] = "application/x-czx";
	media_type[".cur"] = "application/octet-stream";
	media_type[".cxx"] = "text/plain";
	media_type[".datasource"] = "application/xml";
	media_type[".dat"] = "application/octet-stream";
	media_type[".dbproj"] = "text/plain";
	media_type[".def"] = "text/plain";
	media_type[".dcr"] = "application/x-director";
	media_type[".deploy"] = "application/octet-stream";
	media_type[".dgml"] = "application/xml";
	media_type[".der"] = "application/x-x509-ca-cert";
	media_type[".dib"] = "image/bmp";
	media_type[".dir"] = "application/x-director";
	media_type[".dif"] = "video/x-dv";
	media_type[".disco"] = "text/xml";
	media_type[".dll"] = "application/x-msdownload";
	media_type[".divx"] = "video/divx";
	media_type[".dll.config"] = "text/xml";
	media_type[".doc"] = "application/msword";
	media_type[".dlm"] = "text/dlm";
	media_type[".docm"] = "application/vnd.ms-word.document.macroEnabled.12";
	media_type[".dot"] = "application/msword";
	media_type[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	media_type[".dotm"] = "application/vnd.ms-word.template.macroEnabled.12";
	media_type[".dsp"] = "application/octet-stream";
	media_type[".dotx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.template";
	media_type[".dsw"] = "text/plain";
	media_type[".dtsConfig"] = "text/xml";
	media_type[".dtd"] = "text/xml";
	media_type[".dv"] = "video/x-dv";
	media_type[".dwf"] = "drawing/x-dwf";
	media_type[".dvi"] = "application/x-dvi";
	media_type[".dwg"] = "application/acad";
	media_type[".dxf"] = "application/x-dxf";
	media_type[".dwp"] = "application/octet-stream";
	media_type[".dxr"] = "application/x-director";
	media_type[".eml"] = "message/rfc822";
	media_type[".emz"] = "application/octet-stream";
	media_type[".emf"] = "image/emf";
	media_type[".eot"] = "application/vnd.ms-fontobject";
	media_type[".es"] = "application/ecmascript";
	media_type[".eps"] = "application/postscript";
	media_type[".etl"] = "application/etl";
	media_type[".evy"] = "application/envoy";
	media_type[".etx"] = "text/x-setext";
	media_type[".exe"] = "application/x-msdos-program";
	media_type[".f4v"] = "video/mp4";
	media_type[".exe.config"] = "text/xml";
	media_type[".fdf"] = "application/vnd.fdf";
	media_type[".filters"] = "application/xml";
	media_type[".fif"] = "application/fractals";
	media_type[".flac"] = "audio/flac";
	media_type[".fla"] = "application/octet-stream";
	media_type[".flv"] = "video/x-flv";
	media_type[".flr"] = "x-world/x-vrml";
	media_type[".fsscript"] = "application/fsharp-script";
	media_type[".generictest"] = "application/xml";
	media_type[".fsx"] = "application/fsharp-script";
	media_type[".geojson"] = "application/geo+json";
	media_type[".gif"] = "image/gif";
	media_type[".gpx"] = "application/gpx+xml";
	media_type[".gsm"] = "audio/x-gsm";
	media_type[".group"] = "text/x-ms-group";
	media_type[".gtar"] = "application/x-gtar";
	media_type[".gz"] = "application/x-gzip";
	media_type[".h"] = "text/plain";
	media_type[".hdml"] = "text/x-hdml";
	media_type[".hdf"] = "application/x-hdf";
	media_type[".heic"] = "image/heic";
	media_type[".heif"] = "image/heif";
	media_type[".heics"] = "image/heic-sequence";
	media_type[".heifs"] = "image/heif-sequence";
	media_type[".hhk"] = "application/octet-stream";
	media_type[".hhc"] = "application/x-oleobject";
	media_type[".hhp"] = "application/octet-stream";
	media_type[".hpp"] = "text/plain";
	media_type[".hlp"] = "application/winhlp";
	media_type[".hqx"] = "application/mac-binhex40";
	media_type[".htc"] = "text/x-component";
	media_type[".hta"] = "application/hta";
	media_type[".html"] = "text/html";
	media_type[".htm"] = "text/html";
	media_type[".htt"] = "text/webviewhtml";
	media_type[".hxc"] = "application/xml";
	media_type[".hxa"] = "application/xml";
	media_type[".hxd"] = "application/octet-stream";
	media_type[".hxf"] = "application/xml";
	media_type[".hxe"] = "application/xml";
	media_type[".hxh"] = "application/octet-stream";
	media_type[".hxk"] = "application/xml";
	media_type[".hxi"] = "application/octet-stream";
	media_type[".hxq"] = "application/octet-stream";
	media_type[".hxs"] = "application/octet-stream";
	media_type[".hxr"] = "application/octet-stream";
	media_type[".hxt"] = "text/html";
	media_type[".hxw"] = "application/octet-stream";
	media_type[".hxv"] = "application/xml";
	media_type[".hxx"] = "text/plain";
	media_type[".ical"] = "text/calendar";
	media_type[".i"] = "text/plain";
	media_type[".icalendar"] = "text/calendar";
	media_type[".ico"] = "image/x-icon";
	media_type[".idl"] = "text/plain";
	media_type[".ics"] = "text/calendar";
	media_type[".ief"] = "image/ief";
	media_type[".iii"] = "application/x-iphone";
	media_type[".ifb"] = "text/calendar";
	media_type[".inc"] = "text/plain";
	media_type[".inf"] = "application/octet-stream";
	media_type[".ini"] = "text/plain";
	media_type[".ins"] = "application/x-internet-signup";
	media_type[".inl"] = "text/plain";
	media_type[".ipa"] = "application/x-itunes-ipa";
	media_type[".ipg"] = "application/x-itunes-ipg";
	media_type[".ipproj"] = "text/plain";
	media_type[".iqy"] = "text/x-ms-iqy";
	media_type[".ipsw"] = "application/x-itunes-ipsw";
	media_type[".isp"] = "application/x-internet-signup";
	media_type[".isma"] = "application/octet-stream";
	media_type[".ismv"] = "application/octet-stream";
	media_type[".itlp"] = "application/x-itunes-itlp";
	media_type[".ite"] = "application/x-itunes-ite";
	media_type[".itms"] = "application/x-itunes-itms";
	media_type[".itpc"] = "application/x-itunes-itpc";
	media_type[".IVF"] = "video/x-ivf";
	media_type[".jar"] = "application/java-archive";
	media_type[".jck"] = "application/liquidmotion";
	media_type[".jcz"] = "application/liquidmotion";
	media_type[".java"] = "application/octet-stream";
	media_type[".jnlp"] = "application/x-java-jnlp-file";
	media_type[".jpb"] = "application/octet-stream";
	media_type[".jpe"] = "image/jpeg";
	media_type[".jfif"] = "image/pjpeg";
	media_type[".jpg"] = "image/jpeg";
	media_type[".jpeg"] = "image/jpeg";
	media_type[".js"] = "application/javascript";
	media_type[".jsx"] = "text/jscript";
	media_type[".json"] = "application/json";
	media_type[".jsxbin"] = "text/plain";
	media_type[".key"] = "application/x-iwork-keynote-sffkey";
	media_type[".library-ms"] = "application/windows-library+xml";
	media_type[".latex"] = "application/x-latex";
	media_type[".loadtest"] = "application/xml";
	media_type[".lit"] = "application/x-ms-reader";
	media_type[".lpk"] = "application/octet-stream";
	media_type[".lst"] = "text/plain";
	media_type[".lsf"] = "video/x-la-asf";
	media_type[".lsx"] = "video/x-la-asf";
	media_type[".m13"] = "application/x-msmediaview";
	media_type[".lzh"] = "application/octet-stream";
	media_type[".m14"] = "application/x-msmediaview";
	media_type[".m2t"] = "video/vnd.dlna.mpeg-tts";
	media_type[".m1v"] = "video/mpeg";
	media_type[".m2ts"] = "video/vnd.dlna.mpeg-tts";
	media_type[".m3u"] = "audio/x-mpegurl";
	media_type[".m3u8"] = "audio/x-mpegurl";
	media_type[".m2v"] = "video/mpeg";
	media_type[".m4b"] = "audio/m4b";
	media_type[".m4p"] = "audio/m4p";
	media_type[".m4a"] = "audio/x-m4a";
	media_type[".m4v"] = "video/x-m4v";
	media_type[".mac"] = "image/x-macpaint";
	media_type[".m4r"] = "audio/x-m4r";
	media_type[".man"] = "application/x-troff-man";
	media_type[".manifest"] = "application/x-ms-manifest";
	media_type[".mak"] = "text/plain";
	media_type[".master"] = "application/xml";
	media_type[".mbox"] = "application/mbox";
	media_type[".map"] = "text/plain";
	media_type[".mdb"] = "application/x-msaccess";
	media_type[".mde"] = "application/msaccess";
	media_type[".mda"] = "application/msaccess";
	media_type[".me"] = "application/x-troff-me";
	media_type[".mfp"] = "application/x-shockwave-flash";
	media_type[".mht"] = "message/rfc822";
	media_type[".mhtml"] = "message/rfc822";
	media_type[".mdp"] = "application/octet-stream";
	media_type[".midi"] = "audio/mid";
	media_type[".mix"] = "application/octet-stream";
	media_type[".mid"] = "audio/mid";
	media_type[".mk3d"] = "video/x-matroska-3d";
	media_type[".mka"] = "audio/x-matroska";
	media_type[".mkv"] = "video/x-matroska";
	media_type[".mmf"] = "application/x-smaf";
	media_type[".mno"] = "text/xml";
	media_type[".mny"] = "application/x-msmoney";
	media_type[".mod"] = "video/mpeg";
	media_type[".mov"] = "video/quicktime";
	media_type[".movie"] = "video/x-sgi-movie";
	media_type[".mp2"] = "video/mpeg";
	media_type[".mp2v"] = "video/mpeg";
	media_type[".mp3"] = "audio/mpeg";
	media_type[".mp4"] = "video/mp4";
	media_type[".mp4v"] = "video/mp4";
	media_type[".mk"] = "text/plain";
	media_type[".mpa"] = "video/mpeg";
	media_type[".mpe"] = "video/mpeg";
	media_type[".mpf"] = "application/vnd.ms-mediapackage";
	media_type[".mpeg"] = "video/mpeg";
	media_type[".mpp"] = "application/vnd.ms-project";
	media_type[".mpg"] = "video/mpeg";
	media_type[".mqv"] = "video/quicktime";
	media_type[".mpv2"] = "video/mpeg";
	media_type[".msg"] = "application/vnd.ms-outlook";
	media_type[".ms"] = "application/x-troff-ms";
	media_type[".mso"] = "application/octet-stream";
	media_type[".mts"] = "video/vnd.dlna.mpeg-tts";
	media_type[".mtx"] = "application/xml";
	media_type[".mvb"] = "application/x-msmediaview";
	media_type[".msi"] = "application/octet-stream";
	media_type[".mxf"] = "application/mxf";
	media_type[".mxp"] = "application/x-mmxp";
	media_type[".mvc"] = "application/x-miva-compiled";
	media_type[".nsc"] = "video/x-ms-asf";
	media_type[".numbers"] = "application/x-iwork-numbers-sffnumbers";
	media_type[".nc"] = "application/x-netcdf";
	media_type[".ocx"] = "application/octet-stream";
	media_type[".nws"] = "message/rfc822";
	media_type[".odb"] = "application/vnd.oasis.opendocument.database";
	media_type[".oda"] = "application/oda";
	media_type[".odc"] = "application/vnd.oasis.opendocument.chart";
	media_type[".odf"] = "application/vnd.oasis.opendocument.formula";
	media_type[".odh"] = "text/plain";
	media_type[".odg"] = "application/vnd.oasis.opendocument.graphics";
	media_type[".odl"] = "text/plain";
	media_type[".odi"] = "application/vnd.oasis.opendocument.image";
	media_type[".odp"] = "application/vnd.oasis.opendocument.presentation";
	media_type[".odm"] = "application/vnd.oasis.opendocument.text-master";
	media_type[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	media_type[".oga"] = "audio/ogg";
	media_type[".odt"] = "application/vnd.oasis.opendocument.text";
	media_type[".ogg"] = "audio/ogg";
	media_type[".ogx"] = "application/ogg";
	media_type[".ogv"] = "video/ogg";
	media_type[".onea"] = "application/onenote";
	media_type[".one"] = "application/onenote";
	media_type[".onetmp"] = "application/onenote";
	media_type[".onetoc"] = "application/onenote";
	media_type[".onepkg"] = "application/onenote";
	media_type[".opus"] = "audio/ogg; codecs=opus";
	media_type[".orderedtest"] = "application/xml";
	media_type[".onetoc2"] = "application/onenote";
	media_type[".otf"] = "application/font-sfnt";
	media_type[".otg"] = "application/vnd.oasis.opendocument.graphics-template";
	media_type[".osdx"] = "application/opensearchdescription+xml";
	media_type[".oth"] = "application/vnd.oasis.opendocument.text-web";
	media_type[".ots"] = "application/vnd.oasis.opendocument.spreadsheet-template";
	media_type[".ott"] = "application/vnd.oasis.opendocument.text-template";
	media_type[".otp"] = "application/vnd.oasis.opendocument.presentation-template";
	media_type[".oxps"] = "application/oxps";
	media_type[".oxt"] = "application/vnd.openofficeorg.extension";
	media_type[".p12"] = "application/x-pkcs12";
	media_type[".p7b"] = "application/x-pkcs7-certificates";
	media_type[".p7c"] = "application/pkcs7-mime";
	media_type[".p10"] = "application/pkcs10";
	media_type[".p7m"] = "application/pkcs7-mime";
	media_type[".p7r"] = "application/x-pkcs7-certreqresp";
	media_type[".pages"] = "application/x-iwork-pages-sffpages";
	media_type[".pbm"] = "image/x-portable-bitmap";
	media_type[".pcast"] = "application/x-podcast";
	media_type[".p7s"] = "application/pkcs7-signature";
	media_type[".pct"] = "image/pict";
	media_type[".pcz"] = "application/octet-stream";
	media_type[".pdf"] = "application/pdf";
	media_type[".pcx"] = "application/octet-stream";
	media_type[".pfb"] = "application/octet-stream";
	media_type[".pfm"] = "application/octet-stream";
	media_type[".pgm"] = "image/x-portable-graymap";
	media_type[".pic"] = "image/pict";
	media_type[".pfx"] = "application/x-pkcs12";
	media_type[".pict"] = "image/pict";
	media_type[".pkgdef"] = "text/plain";
	media_type[".pko"] = "application/vnd.ms-pki.pko";
	media_type[".pls"] = "audio/scpls";
	media_type[".pkgundef"] = "text/plain";
	media_type[".pma"] = "application/x-perfmon";
	media_type[".pmc"] = "application/x-perfmon";
	media_type[".pmr"] = "application/x-perfmon";
	media_type[".pmw"] = "application/x-perfmon";
	media_type[".pml"] = "application/x-perfmon";
	media_type[".png"] = "image/x-png";
	media_type[".pnm"] = "image/x-portable-anymap";
	media_type[".pntg"] = "image/x-macpaint";
	media_type[".pnz"] = "image/png";
	media_type[".pnt"] = "image/x-macpaint";
	media_type[".pot"] = "application/vnd.ms-powerpoint";
	media_type[".potm"] = "application/vnd.ms-powerpoint.template.macroEnabled.12";
	media_type[".ppa"] = "application/vnd.ms-powerpoint";
	media_type[".potx"] = "application/vnd.openxmlformats-officedocument.presentationml.template";
	media_type[".ppam"] = "application/vnd.ms-powerpoint.addin.macroEnabled.12";
	media_type[".ppm"] = "image/x-portable-pixmap";
	media_type[".ppsm"] = "application/vnd.ms-powerpoint.slideshow.macroEnabled.12";
	media_type[".ppsx"] = "application/vnd.openxmlformats-officedocument.presentationml.slideshow";
	media_type[".pps"] = "application/vnd.ms-powerpoint";
	media_type[".ppt"] = "application/vnd.ms-powerpoint";
	media_type[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	media_type[".prf"] = "application/pics-rules";
	media_type[".pptm"] = "application/vnd.ms-powerpoint.presentation.macroEnabled.12";
	media_type[".prm"] = "application/octet-stream";
	media_type[".ps"] = "application/postscript";
	media_type[".psc1"] = "application/PowerShell";
	media_type[".prx"] = "application/octet-stream";
	media_type[".psd"] = "application/octet-stream";
	media_type[".psm"] = "application/octet-stream";
	media_type[".psp"] = "application/octet-stream";
	media_type[".psess"] = "application/xml";
	media_type[".pst"] = "application/vnd.ms-outlook";
	media_type[".pwz"] = "application/vnd.ms-powerpoint";
	media_type[".qht"] = "text/x-html-insertion";
	media_type[".pub"] = "application/x-mspublisher";
	media_type[".qhtm"] = "text/x-html-insertion";
	media_type[".qti"] = "image/x-quicktime";
	media_type[".qtif"] = "image/x-quicktime";
	media_type[".qt"] = "video/quicktime";
	media_type[".qtl"] = "application/x-quicktimeplayer";
	media_type[".ra"] = "audio/x-pn-realaudio";
	media_type[".ram"] = "audio/x-pn-realaudio";
	media_type[".qxd"] = "application/octet-stream";
	media_type[".rar"] = "application/x-rar-compressed";
	media_type[".rat"] = "application/rat-file";
	media_type[".rc"] = "text/plain";
	media_type[".ras"] = "image/x-cmu-raster";
	media_type[".rc2"] = "text/plain";
	media_type[".rdlc"] = "application/xml";
	media_type[".reg"] = "text/plain";
	media_type[".rct"] = "text/plain";
	media_type[".resx"] = "application/xml";
	media_type[".rgb"] = "image/x-rgb";
	media_type[".rgs"] = "text/plain";
	media_type[".rf"] = "image/vnd.rn-realflash";
	media_type[".rm"] = "application/vnd.rn-realmedia";
	media_type[".rmp"] = "application/vnd.rn-rn_music_package";
	media_type[".rmvb"] = "application/vnd.rn-realmedia-vbr";
	media_type[".rmi"] = "audio/mid";
	media_type[".roff"] = "application/x-troff";
	media_type[".rqy"] = "text/x-ms-rqy";
	media_type[".rtf"] = "application/rtf";
	media_type[".rpm"] = "audio/x-pn-realaudio-plugin";
	media_type[".rtx"] = "text/richtext";
	media_type[".ruleset"] = "application/xml";
	media_type[".s"] = "text/plain";
	media_type[".rvt"] = "application/octet-stream";
	media_type[".safariextz"] = "application/x-safari-safariextz";
	media_type[".scd"] = "application/x-msschedule";
	media_type[".sct"] = "text/scriptlet";
	media_type[".scr"] = "text/plain";
	media_type[".shtml"] = "text/html";
	media_type[".sd2"] = "audio/x-sd2";
	media_type[".sdp"] = "application/sdp";
	media_type[".sgml"] = "text/sgml";
	media_type[".searchConnector-ms"] = "application/windows-search-connector+xml";
	media_type[".setpay"] = "application/set-payment-initiation";
	media_type[".sea"] = "application/octet-stream";
	media_type[".settings"] = "application/xml";
	media_type[".sgimb"] = "application/x-sgimb";
	media_type[".setreg"] = "application/set-registration-initiation";
	media_type[".shar"] = "application/x-shar";
	media_type[".sitemap"] = "application/xml";
	media_type[".sit"] = "application/x-stuffit";
	media_type[".sh"] = "application/x-sh";
	media_type[".sldx"] = "application/vnd.openxmlformats-officedocument.presentationml.slide";
	media_type[".sldm"] = "application/vnd.ms-powerpoint.slide.macroEnabled.12";
	media_type[".skp"] = "application/x-koan";
	media_type[".skin"] = "application/xml";
	media_type[".slupkg-ms"] = "application/x-ms-license";
	media_type[".sln"] = "text/plain";
	media_type[".slk"] = "application/vnd.ms-excel";
	media_type[".smi"] = "application/octet-stream";
	media_type[".smz"] = "audio/x-smd";
	media_type[".smx"] = "audio/x-smd";
	media_type[".smd"] = "audio/x-smd";
	media_type[".snd"] = "audio/basic";
	media_type[".snp"] = "application/octet-stream";
	media_type[".sql"] = "application/sql";
	media_type[".sol"] = "text/plain";
	media_type[".snippet"] = "application/xml";
	media_type[".spc"] = "application/x-pkcs7-certificates";
	media_type[".spl"] = "application/futuresplash";
	media_type[".spx"] = "audio/ogg";
	media_type[".sor"] = "text/plain";
	media_type[".srf"] = "text/plain";
	media_type[".SSISDeploymentManifest"] = "text/xml";
	media_type[".ssm"] = "application/streamingmedia";
	media_type[".sst"] = "application/vnd.ms-pki.certstore";
	media_type[".stl"] = "application/vnd.ms-pki.stl";
	media_type[".sv4cpio"] = "application/x-sv4cpio";
	media_type[".src"] = "application/x-wais-source";
	media_type[".svc"] = "application/xml";
	media_type[".svg"] = "image/svg+xml";
	media_type[".sv4crc"] = "application/x-sv4crc";
	media_type[".step"] = "application/step";
	media_type[".stp"] = "application/step";
	media_type[".swf"] = "application/x-shockwave-flash";
	media_type[".tar"] = "application/x-tar";
	media_type[".tcl"] = "application/x-tcl";
	media_type[".t"] = "application/x-troff";
	media_type[".testsettings"] = "application/xml";
	media_type[".tex"] = "application/x-tex";
	media_type[".texi"] = "application/x-texinfo";
	media_type[".texinfo"] = "application/x-texinfo";
	media_type[".testrunconfig"] = "application/xml";
	media_type[".thmx"] = "application/vnd.ms-officetheme";
	media_type[".thn"] = "application/octet-stream";
	media_type[".tgz"] = "application/x-compressed";
	media_type[".tiff"] = "image/tiff";
	media_type[".tlh"] = "text/plain";
	media_type[".tif"] = "image/tiff";
	media_type[".toc"] = "application/octet-stream";
	media_type[".tr"] = "application/x-troff";
	media_type[".tli"] = "text/plain";
	media_type[".trx"] = "application/xml";
	media_type[".ts"] = "video/vnd.dlna.mpeg-tts";
	media_type[".trm"] = "application/x-msterminal";
	media_type[".ttf"] = "application/font-sfnt";
	media_type[".tts"] = "video/vnd.dlna.mpeg-tts";
	media_type[".tsv"] = "text/tab-separated-values";
	media_type[".u32"] = "application/octet-stream";
	media_type[".uls"] = "text/iuls";
	media_type[".txt"] = "text/plain";
	media_type[".ustar"] = "application/x-ustar";
	media_type[".vb"] = "text/plain";
	media_type[".user"] = "text/plain";
	media_type[".vbk"] = "video/mpeg";
	media_type[".vbproj"] = "text/plain";
	media_type[".vbdproj"] = "text/plain";
	media_type[".vcf"] = "text/x-vcard";
	media_type[".vcproj"] = "application/xml";
	media_type[".vbs"] = "text/vbscript";
	media_type[".vcxproj"] = "application/xml";
	media_type[".vddproj"] = "text/plain";
	media_type[".vcs"] = "text/plain";
	media_type[".vdproj"] = "text/plain";
	media_type[".vdx"] = "application/vnd.ms-visio.viewer";
	media_type[".vdp"] = "text/plain";
	media_type[".vml"] = "text/xml";
	media_type[".vsct"] = "text/xml";
	media_type[".vsd"] = "application/vnd.visio";
	media_type[".vscontent"] = "application/xml";
	media_type[".vsi"] = "application/ms-vsi";
	media_type[".vsixlangpack"] = "text/xml";
	media_type[".vsixmanifest"] = "text/xml";
	media_type[".vsix"] = "application/vsix";
	media_type[".vsmdi"] = "application/xml";
	media_type[".vss"] = "application/vnd.visio";
	media_type[".vsscc"] = "text/plain";
	media_type[".vspscc"] = "text/plain";
	media_type[".vssettings"] = "text/xml";
	media_type[".vst"] = "application/vnd.visio";
	media_type[".vssscc"] = "text/plain";
	media_type[".vsto"] = "application/x-ms-vsto";
	media_type[".vstemplate"] = "text/xml";
	media_type[".vsx"] = "application/vnd.visio";
	media_type[".vsw"] = "application/vnd.visio";
	media_type[".vtt"] = "text/vtt";
	media_type[".vtx"] = "application/vnd.visio";
	media_type[".wav"] = "audio/wav";
	media_type[".wave"] = "audio/wav";
	media_type[".wax"] = "audio/x-ms-wax";
	media_type[".wbk"] = "application/msword";
	media_type[".wbmp"] = "image/vnd.wap.wbmp";
	media_type[".wasm"] = "application/wasm";
	media_type[".wcm"] = "application/vnd.ms-works";
	media_type[".wdb"] = "application/vnd.ms-works";
	media_type[".webarchive"] = "application/x-safari-webarchive";
	media_type[".webm"] = "video/webm";
	media_type[".webp"] = "image/webp";
	media_type[".wdp"] = "image/vnd.ms-photo";
	media_type[".webtest"] = "application/xml";
	media_type[".wiz"] = "application/msword";
	media_type[".wks"] = "application/vnd.ms-works";
	media_type[".WLMP"] = "application/wlmoviemaker";
	media_type[".wiq"] = "application/xml";
	media_type[".wlpginstall"] = "application/x-wlpg-detect";
	media_type[".wm"] = "video/x-ms-wm";
	media_type[".wma"] = "audio/x-ms-wma";
	media_type[".wmd"] = "application/x-ms-wmd";
	media_type[".wlpginstall3"] = "application/x-wlpg3-detect";
	media_type[".wmf"] = "application/x-msmetafile";
	media_type[".wmlc"] = "application/vnd.wap.wmlc";
	media_type[".wmls"] = "text/vnd.wap.wmlscript";
	media_type[".wml"] = "text/vnd.wap.wml";
	media_type[".wmlsc"] = "application/vnd.wap.wmlscriptc";
	media_type[".wmv"] = "video/x-ms-wmv";
	media_type[".wmx"] = "video/x-ms-wmx";
	media_type[".wmp"] = "video/x-ms-wmp";
	media_type[".wmz"] = "application/x-ms-wmz";
	media_type[".woff2"] = "application/font-woff2";
	media_type[".wpl"] = "application/vnd.ms-wpl";
	media_type[".wps"] = "application/vnd.ms-works";
	media_type[".woff"] = "application/font-woff";
	media_type[".wrl"] = "x-world/x-vrml";
	media_type[".wrz"] = "x-world/x-vrml";
	media_type[".wri"] = "application/x-mswrite";
	media_type[".wsc"] = "text/scriptlet";
	media_type[".wvx"] = "video/x-ms-wvx";
	media_type[".x"] = "application/directx";
	media_type[".wsdl"] = "text/xml";
	media_type[".xaf"] = "x-world/x-vrml";
	media_type[".xap"] = "application/x-silverlight-app";
	media_type[".xbap"] = "application/x-ms-xbap";
	media_type[".xaml"] = "application/xaml+xml";
	media_type[".xbm"] = "image/x-xbitmap";
	media_type[".xht"] = "application/xhtml+xml";
	media_type[".xhtml"] = "application/xhtml+xml";
	media_type[".xdr"] = "text/plain";
	media_type[".xla"] = "application/vnd.ms-excel";
	media_type[".xlc"] = "application/vnd.ms-excel";
	media_type[".xld"] = "application/vnd.ms-excel";
	media_type[".xlam"] = "application/vnd.ms-excel.addin.macroEnabled.12";
	media_type[".xlk"] = "application/vnd.ms-excel";
	media_type[".xlm"] = "application/vnd.ms-excel";
	media_type[".xls"] = "application/vnd.ms-excel";
	media_type[".xll"] = "application/vnd.ms-excel";
	media_type[".xlsb"] = "application/vnd.ms-excel.sheet.binary.macroEnabled.12";
	media_type[".xlsm"] = "application/vnd.ms-excel.sheet.macroEnabled.12";
	media_type[".xlt"] = "application/vnd.ms-excel";
	media_type[".xltm"] = "application/vnd.ms-excel.template.macroEnabled.12";
	media_type[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	media_type[".xltx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
	media_type[".xml"] = "text/xml";
	media_type[".xmp"] = "application/octet-stream";
	media_type[".xlw"] = "application/vnd.ms-excel";
	media_type[".xmta"] = "application/xml";
	media_type[".XOML"] = "text/plain";
	media_type[".xpm"] = "image/x-xpixmap";
	media_type[".xof"] = "x-world/x-vrml";
	media_type[".xps"] = "application/vnd.ms-xpsdocument";
	media_type[".xsc"] = "application/xml";
	media_type[".xsd"] = "text/xml";
	media_type[".xrm-ms"] = "text/xml";
	media_type[".xsf"] = "text/xml";
	media_type[".xslt"] = "text/xml";
	media_type[".xsn"] = "application/octet-stream";
	media_type[".xsl"] = "text/xml";
	media_type[".xss"] = "application/xml";
	media_type[".xspf"] = "application/xspf+xml";
	media_type[".xwd"] = "image/x-xwindowdump";
	media_type[".xtp"] = "application/octet-stream";
	media_type[".zip"] = "application/x-zip-compressed";
	media_type[".z"] = "application/x-compress";

	return getMediaType(file);
}
