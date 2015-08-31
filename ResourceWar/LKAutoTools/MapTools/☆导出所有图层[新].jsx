// Copyright 2005.  Adobe Systems, Incorporated.  All rights reserved.
// This script will export each layer in the document to a separate file.
// Written by Naoki Hada
// ZStrings and auto layout by Tom Ruark

/// enable double clicking from the Macintosh Finder or the Windows Explorer
#target photoshop

// in case we double clicked the file
app.bringToFront();

// debug level: 0-2 (0:disable, 1:break on error, 2:break at beginning)
// $.level = 0;
// debugger; // launch debugger on next line

// on localized builds we pull the $$$/Strings from a .dat file, see documentation for more details
$.localize = true;

var logfile;
var totle_num = 0;
//=================================================================
// Globals
//=================================================================

// UI strings to be localized
var strTitle = "保存所有图层";
var strButtonRun = "开始运行";
var strButtonCancel = "取消";
var strHelpText = "请指定将每个图层存储为文件时的格式和位置。";
var strLabelDestination = "目标:";
var strButtonBrowse = "浏览...";
var strLabelFileNamePrefix = "文件名前缀:";
var strCheckboxVisibleOnly = "仅限可视图层";
var strLabelFileType = "文件类型:";
var strCheckboxIncludeICCProfile = "包含 ICC 配置文件";
var strJPEGOptions = "JPEG 选项:";
var strLabelQuality = "品质:";
var strPSDOptions = "PSD 选项:";
var strCheckboxMaximizeCompatibility = "最大兼容";
var strTIFFOptions = "TIFF 选项:";
var strLabelImageCompression = "图像压缩:";
var strNone = "无";
var strPDFOptions = "PDF 选项:";
var strLabelEncoding = "编码:";
var strTargaOptions = "Targa 选项:";
var strLabelDepth = "深度:";
var strRadiobutton16bit = "16 位";
var strRadiobutton24bit = "24 位";
var strRadiobutton32bit = "32 位";
var strPngOptions = "PNG 选项:";
var strLabelJX = "交错:";
var strJXNone = "无";
var strJX = "交错";
var strBMPOptions = "BMP 选项:";
var strAlertSpecifyDestination = "请指定目标。";
var strAlertDestinationNotExist = "目标不存在。";
var strTitleSelectDestination = "选择目标";
var strAlertDocumentMustBeOpened = "必须打开文档才能导出！";
var strAlertNeedMultipleLayers = "文档需要有多个图层才能导出！";
var strAlertWasSuccessful = ": 成功。";
var strUnexpectedError = "意外错误";

// the drop down list indexes for file type
var bmpIndex = 0; 
var jpegIndex = 1;
var pdfIndex = 2;
var psdIndex = 3;
var targaIndex = 4;
var tiffIndex = 5;
var pngIndex = 6;

// the drop down list indexes for tiff compression
var compNoneIndex = 0;
var compLZWIndex = 1;
var compZIPIndex = 2;
var compJPEGIndex = 3;


///////////////////////////////////////////////////////////////////////////////
// Dispatch
///////////////////////////////////////////////////////////////////////////////


main();



///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Function: main
// Usage: the core routine for this script
// Input: <none>
// Return: <none>
///////////////////////////////////////////////////////////////////////////////
function main() {
    if ( app.documents.length <= 0 ) {
        alert( strAlertDocumentMustBeOpened );
        return;
    }

    var exportInfo = new Object();
    
    initExportInfo(exportInfo);
    
    if (false == settingDialog(exportInfo)) {
		return; // quit
	}

    try {
        var docName = app.activeDocument.name;  // save the app.activeDocument name before duplicate.

        var layerCount = app.documents[docName].layers.length;
        var layerSetsCount = app.documents[docName].layerSets.length;

        if ((layerCount <= 1)&&(layerSetsCount <= 0)) {
            alert ( strAlertNeedMultipleLayers );
        } else {
			logfile = new File("d:/"+docName+".txt"); 
			logfile.open( "e" );
			totle_num = 0;

            app.activeDocument = app.documents[docName];
            var duppedDocument = app.activeDocument;
//            duppedDocument.activeLayer = duppedDocument.layers[duppedDocument.layers.length-1]; // for removing
            exportChildren(duppedDocument, app.documents[docName], exportInfo, duppedDocument, exportInfo.fileNamePrefix);
			logfile.close();
            alert(strTitle + "(" + totle_num + "个)" +strAlertWasSuccessful);
        }
    } catch (e) {
        alert(e);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Function: settingDialog
// Usage: pop the ui and get user settings
// Input: exportInfo object containing our parameters
// Return: on ok, the dialog info is set to the exportInfo object
///////////////////////////////////////////////////////////////////////////////
function settingDialog(exportInfo) {
    dlgMain = new Window("dialog", strTitle);

	dlgMain.orientation = 'column';
	dlgMain.alignChildren = 'left';
	
	// -- top of the dialog, first line
    dlgMain.add("statictext", undefined, strLabelDestination);

	// -- two groups, one for left and one for right ok, cancel
	dlgMain.grpTop = dlgMain.add("group");
	dlgMain.grpTop.orientation = 'row';
	dlgMain.grpTop.alignChildren = 'top';
	dlgMain.grpTop.alignment = 'fill';

	// -- group top left 
	dlgMain.grpTopLeft = dlgMain.grpTop.add("group");
	dlgMain.grpTopLeft.orientation = 'column';
	dlgMain.grpTopLeft.alignChildren = 'left';
	dlgMain.grpTopLeft.alignment = 'fill';
	
	// -- the second line in the dialog
	dlgMain.grpSecondLine = dlgMain.grpTopLeft.add("group");
	dlgMain.grpSecondLine.orientation = 'row';
	dlgMain.grpSecondLine.alignChildren = 'center';

    dlgMain.etDestination = dlgMain.grpSecondLine.add("edittext", undefined, exportInfo.destination.toString());
    dlgMain.etDestination.preferredSize.width = 160;

    dlgMain.btnBrowse = dlgMain.grpSecondLine.add("button", undefined, strButtonBrowse);
    dlgMain.btnBrowse.onClick = function() {
		var defaultFolder = dlgMain.etDestination.text;
		var testFolder = new Folder(dlgMain.etDestination.text);
		if (!testFolder.exists) {
			defaultFolder = "~";
		}
		var selFolder = Folder.selectDialog(strTitleSelectDestination, defaultFolder);
		if ( selFolder != null ) {
	        dlgMain.etDestination.text = selFolder.fsName;
	    }
		dlgMain.defaultElement.active = true;
	}

	// -- the third line in the dialog
    dlgMain.grpTopLeft.add("statictext", undefined, strLabelFileNamePrefix);

	// -- the fourth line in the dialog
    dlgMain.etFileNamePrefix = dlgMain.grpTopLeft.add("edittext", undefined, exportInfo.fileNamePrefix.toString());
    dlgMain.etFileNamePrefix.alignment = 'fill';

	// -- the fifth line in the dialog
    dlgMain.cbVisible = dlgMain.grpTopLeft.add("checkbox", undefined, strCheckboxVisibleOnly);
    dlgMain.cbVisible.value = exportInfo.visibleOnly;

	// -- the sixth line is the panel
    dlgMain.pnlFileType = dlgMain.grpTopLeft.add("panel", undefined, strLabelFileType);
	dlgMain.pnlFileType.alignment = 'fill';
    
    // -- now a dropdown list
    dlgMain.ddFileType = dlgMain.pnlFileType.add("dropdownlist");
    dlgMain.ddFileType.preferredSize.width = 100;
    dlgMain.ddFileType.alignment = 'left';

    dlgMain.ddFileType.add("item", "BMP");
    dlgMain.ddFileType.add("item", "JPEG");
    dlgMain.ddFileType.add("item", "PDF");
	dlgMain.ddFileType.add("item", "PSD");
    dlgMain.ddFileType.add("item", "Targa");
    dlgMain.ddFileType.add("item", "TIFF");
    dlgMain.ddFileType.add("item", "PNG");

	dlgMain.ddFileType.onChange = function() {
		hideAllFileTypePanel();
		switch(this.selection.index) {
			case bmpIndex:	
				dlgMain.pnlFileType.pnlOptions.text = strBMPOptions;
				dlgMain.pnlFileType.pnlOptions.grpBMPOptions.show();	
				break;
			case jpegIndex:	
				dlgMain.pnlFileType.pnlOptions.text = strJPEGOptions;
				dlgMain.pnlFileType.pnlOptions.grpJPEGOptions.show();	
				break;
			case tiffIndex:	
				dlgMain.pnlFileType.pnlOptions.text = strTIFFOptions;
				dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.show();	
				break;
			case pdfIndex:	
				dlgMain.pnlFileType.pnlOptions.text = strPDFOptions;
				dlgMain.pnlFileType.pnlOptions.grpPDFOptions.show();	
				break;
			case targaIndex:
				dlgMain.pnlFileType.pnlOptions.text = strTargaOptions;
				dlgMain.pnlFileType.pnlOptions.grpTargaOptions.show();	
				break;
			case psdIndex:	
				dlgMain.pnlFileType.pnlOptions.text = strPSDOptions;
				dlgMain.pnlFileType.pnlOptions.grpPSDOptions.show();	
				break;
			case pngIndex:
			default:		
				dlgMain.pnlFileType.pnlOptions.text = strPngOptions;
				dlgMain.pnlFileType.pnlOptions.grpPngOptions.show();	
				break;
		}
	}
	    
    dlgMain.ddFileType.items[exportInfo.fileType].selected = true;

	// -- now after all the radio buttons
    dlgMain.cbIcc = dlgMain.pnlFileType.add("checkbox", undefined, strCheckboxIncludeICCProfile);
    dlgMain.cbIcc.value = exportInfo.icc;
    dlgMain.cbIcc.alignment = 'left';

	// -- now the options panel that changes
    dlgMain.pnlFileType.pnlOptions = dlgMain.pnlFileType.add("panel", undefined, "Options");
    dlgMain.pnlFileType.pnlOptions.alignment = 'fill';
    dlgMain.pnlFileType.pnlOptions.orientation = 'stack';
    dlgMain.pnlFileType.pnlOptions.preferredSize.height = 100;

	// PSD options
    dlgMain.pnlFileType.pnlOptions.grpPSDOptions = dlgMain.pnlFileType.pnlOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpPSDOptions.cbMax = dlgMain.pnlFileType.pnlOptions.grpPSDOptions.add("checkbox", undefined, strCheckboxMaximizeCompatibility);
    dlgMain.pnlFileType.pnlOptions.grpPSDOptions.cbMax.value = exportInfo.psdMaxComp;

	// JPEG options
    dlgMain.pnlFileType.pnlOptions.grpJPEGOptions = dlgMain.pnlFileType.pnlOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpJPEGOptions.add("statictext", undefined, strLabelQuality);
    dlgMain.pnlFileType.pnlOptions.grpJPEGOptions.etQuality = dlgMain.pnlFileType.pnlOptions.grpJPEGOptions.add("edittext", undefined, exportInfo.jpegQuality.toString());
    dlgMain.pnlFileType.pnlOptions.grpJPEGOptions.etQuality.preferredSize.width = 30;

	// TIFF options
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions = dlgMain.pnlFileType.pnlOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.orientation = 'column';
    
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression = dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.alignment = 'left';
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.add("statictext", undefined, strLabelImageCompression);
    

    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.ddCompression = dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.add("dropdownlist");
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.ddCompression.add("item", strNone);
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.ddCompression.add("item", "LZW");
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.ddCompression.add("item", "ZIP");
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.ddCompression.add("item", "JPEG");
    
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.ddCompression.onChange = function() {
		if (this.selection.index == compJPEGIndex) {
			dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.stQuality.enabled = true;
			dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.etQuality.enabled = true;
		} else {
			dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.stQuality.enabled = false;
			dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.etQuality.enabled = false;
		}
    }

	dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality = dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.alignment = 'left';
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.stQuality = dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.add("statictext", undefined, strLabelQuality);
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.etQuality = dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.add("edittext", undefined, exportInfo.tiffJpegQuality.toString());
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.etQuality.preferredSize.width = 30;

	var index;
    switch (exportInfo.tiffCompression) {
		case TIFFEncoding.NONE:     index = compNoneIndex; break;
        case TIFFEncoding.TIFFLZW:  index = compLZWIndex; break;
        case TIFFEncoding.TIFFZIP:  index = compZIPIndex; break;
        case TIFFEncoding.JPEG:     index = compJPEGIndex; break;
        default: index = compNoneIndex;    break;
    }

    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.ddCompression.items[index].selected = true;

	if (TIFFEncoding.JPEG != exportInfo.tiffCompression) { // if not JPEG
		dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.stQuality.enabled = false;
		dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.etQuality.enabled = false;
    }
    

	// PDF options
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions = dlgMain.pnlFileType.pnlOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.orientation = 'column';

    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression = dlgMain.pnlFileType.pnlOptions.grpPDFOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.alignment = 'left';
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.add("statictext", undefined, strLabelEncoding);

    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.rbZip = dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.add("radiobutton", undefined, "ZIP");
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.rbZip.onClick = function() {
		dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.stQuality.enabled = false;   
		dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.etQuality.enabled = false;   
	}

    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.rbJpeg = dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.add("radiobutton", undefined, "JPEG");
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.rbJpeg.onClick = function() {
		dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.stQuality.enabled = true;   
		dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.etQuality.enabled = true;   
	}
	
	dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality = dlgMain.pnlFileType.pnlOptions.grpPDFOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.alignment = 'left';
    
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.stQuality = dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.add("statictext", undefined, strLabelQuality);

    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.etQuality = dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.add("edittext", undefined, exportInfo.pdfJpegQuality.toString());
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.etQuality.preferredSize.width = 30;

    switch (exportInfo.pdfEncoding) {
        case PDFEncoding.PDFZIP: 
			dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.rbZip.value  = true;    break;
        case PDFEncoding.JPEG:
        default: 
			dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.rbJpeg.value = true;    break;
    }
    
    if (PDFEncoding.JPEG != exportInfo.pdfEncoding) {
        dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.stQuality.enabled = false;
        dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.etQuality.enabled = false;
    }

	// Targa options
	dlgMain.pnlFileType.pnlOptions.grpTargaOptions = dlgMain.pnlFileType.pnlOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpTargaOptions.add("statictext", undefined, strLabelDepth);

    dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb16bit = dlgMain.pnlFileType.pnlOptions.grpTargaOptions.add( "radiobutton", undefined, strRadiobutton16bit);
    dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb24bit = dlgMain.pnlFileType.pnlOptions.grpTargaOptions.add( "radiobutton", undefined, strRadiobutton24bit);
    dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb32bit = dlgMain.pnlFileType.pnlOptions.grpTargaOptions.add( "radiobutton", undefined, strRadiobutton32bit);

    switch (exportInfo.targaDepth) {
        case TargaBitsPerPixels.SIXTEEN:     dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb16bit.value = true;   break;
        case TargaBitsPerPixels.TWENTYFOUR:  dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb24bit.value = true;   break;
        case TargaBitsPerPixels.THIRTYTWO:   dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb32bit.value = true;   break;
        default: dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb24bit.value = true;   break;
    }
    
	// Png options
	dlgMain.pnlFileType.pnlOptions.grpPngOptions = dlgMain.pnlFileType.pnlOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpPngOptions.add("statictext", undefined, strLabelJX);

    dlgMain.pnlFileType.pnlOptions.grpPngOptions.JXNone = dlgMain.pnlFileType.pnlOptions.grpPngOptions.add( "radiobutton", undefined, strJXNone);
    dlgMain.pnlFileType.pnlOptions.grpPngOptions.JX = dlgMain.pnlFileType.pnlOptions.grpPngOptions.add( "radiobutton", undefined, strJX);

	if( exportInfo.jx )
	{
		dlgMain.pnlFileType.pnlOptions.grpPngOptions.JXNone.value = false;
		dlgMain.pnlFileType.pnlOptions.grpPngOptions.JX.value = true;
	}
	else
	{
		dlgMain.pnlFileType.pnlOptions.grpPngOptions.JXNone.value = true;
		dlgMain.pnlFileType.pnlOptions.grpPngOptions.JX.value = false;
	}


	// BMP options
    dlgMain.pnlFileType.pnlOptions.grpBMPOptions = dlgMain.pnlFileType.pnlOptions.add("group");
    dlgMain.pnlFileType.pnlOptions.grpBMPOptions.add("statictext", undefined, strLabelDepth);

    dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb16bit = dlgMain.pnlFileType.pnlOptions.grpBMPOptions.add( "radiobutton", undefined, strRadiobutton16bit);
    dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb24bit = dlgMain.pnlFileType.pnlOptions.grpBMPOptions.add( "radiobutton", undefined, strRadiobutton24bit);
    dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb32bit = dlgMain.pnlFileType.pnlOptions.grpBMPOptions.add( "radiobutton", undefined, strRadiobutton32bit);

    switch (exportInfo.bmpDepth) {
        case BMPDepthType.SIXTEEN:   dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb16bit.value = true;   break;
        case BMPDepthType.TWENTYFOUR:dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb24bit.value = true;   break;
        case BMPDepthType.THIRTYTWO: dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb32bit.value = true;   break;
        default: dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb24bit.value = true;   break;
    }

	// the right side of the dialog, the ok and cancel buttons
	dlgMain.grpTopRight = dlgMain.grpTop.add("group");
	dlgMain.grpTopRight.orientation = 'column';
	dlgMain.grpTopRight.alignChildren = 'fill';
	
	dlgMain.btnRun = dlgMain.grpTopRight.add("button", undefined, strButtonRun );

    dlgMain.btnRun.onClick = function() {
		// check if the setting is properly
		var destination = dlgMain.etDestination.text;
		if (destination.length == 0) {
	        alert(strAlertSpecifyDestination);
			return;
		}
		var testFolder = new Folder(destination);
		if (!testFolder.exists) {
	        alert(strAlertDestinationNotExist);
			return;
		}
    
		dlgMain.close(true);
	}

	dlgMain.btnCancel = dlgMain.grpTopRight.add("button", undefined, strButtonCancel );

    dlgMain.btnCancel.onClick = function() { 
		dlgMain.close(false); 
	}

	dlgMain.defaultElement = dlgMain.btnRun;
	dlgMain.cancelElement = dlgMain.btnCancel;

   	// the bottom of the dialog
	dlgMain.grpBottom = dlgMain.add("group");
	dlgMain.grpBottom.orientation = 'column';
	dlgMain.grpBottom.alignChildren = 'left';
	dlgMain.grpBottom.alignment = 'fill';
    
    dlgMain.pnlHelp = dlgMain.grpBottom.add("panel");
    dlgMain.pnlHelp.alignment = 'fill';

    dlgMain.etHelp = dlgMain.pnlHelp.add("statictext", undefined, strHelpText, {multiline:true});
    dlgMain.etHelp.alignment = 'fill';

    dlgMain.center();
    
    var result = dlgMain.show();
    
    if (false == result) {
		return result;  // close to quit
	}
    
    // get setting from dialog
    exportInfo.destination = dlgMain.etDestination.text;
    exportInfo.fileNamePrefix = dlgMain.etFileNamePrefix.text;
    exportInfo.visibleOnly = dlgMain.cbVisible.value;
    exportInfo.fileType = dlgMain.ddFileType.selection.index;
    exportInfo.icc = dlgMain.cbIcc.value;
    exportInfo.jpegQuality = dlgMain.pnlFileType.pnlOptions.grpJPEGOptions.etQuality.text;
    exportInfo.psdMaxComp = dlgMain.pnlFileType.pnlOptions.grpPSDOptions.cbMax.value;
    index = dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpCompression.ddCompression.selection.index;
    if (index == compNoneIndex) {
		exportInfo.tiffCompression = TIFFEncoding.NONE;
	}
    if (index == compLZWIndex) {
		exportInfo.tiffCompression = TIFFEncoding.TIFFLZW;
	}
    if (index == compZIPIndex) {
		exportInfo.tiffCompression = TIFFEncoding.TIFFZIP;
	}
    if (index == compJPEGIndex) {
		exportInfo.tiffCompression = TIFFEncoding.JPEG;
	}
    exportInfo.tiffJpegQuality = dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.grpQuality.etQuality.text;
    if (dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.rbZip.value) {
		exportInfo.pdfEncoding = PDFEncoding.PDFZIP;
	}
    if (dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpCompression.rbJpeg.value) {
		exportInfo.pdfEncoding = PDFEncoding.JPEG;
	}
    exportInfo.pdfJpegQuality = dlgMain.pnlFileType.pnlOptions.grpPDFOptions.grpQuality.etQuality.text;
    if (dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb16bit.value) {
		exportInfo.targaDepth = TargaBitsPerPixels.SIXTEEN;
	}
    if (dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb24bit.value) {
		exportInfo.targaDepth = TargaBitsPerPixels.TWENTYFOUR;
	}
    if (dlgMain.pnlFileType.pnlOptions.grpTargaOptions.rb32bit.value) {
		exportInfo.targaDepth = TargaBitsPerPixels.THIRTYTWO;
	}
    if (dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb16bit.value) {
		exportInfo.bmpDepth = BMPDepthType.SIXTEEN;
	}
    if (dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb24bit.value) {
		exportInfo.bmpDepth = BMPDepthType.TWENTYFOUR;
	}
    if (dlgMain.pnlFileType.pnlOptions.grpBMPOptions.rb32bit.value) { 
		exportInfo.bmpDepth = BMPDepthType.THIRTYTWO;
	}
	
	if( dlgMain.pnlFileType.pnlOptions.grpPngOptions.JX.value )
	{
		exportInfo.jx = true;
	}
	else
	{
		exportInfo.jx = false;
	}

    return result;
}


///////////////////////////////////////////////////////////////////////////////
// Function: hideAllFileTypePanel
// Usage: hide all the panels in the common actions
// Input: <none>, dlgMain is a global for this script
// Return: <none>, all panels are now hidden
///////////////////////////////////////////////////////////////////////////////
function hideAllFileTypePanel() {
    dlgMain.pnlFileType.pnlOptions.grpPSDOptions.hide();
    dlgMain.pnlFileType.pnlOptions.grpJPEGOptions.hide();
    dlgMain.pnlFileType.pnlOptions.grpTIFFOptions.hide();
    dlgMain.pnlFileType.pnlOptions.grpPDFOptions.hide();
    dlgMain.pnlFileType.pnlOptions.grpTargaOptions.hide();
    dlgMain.pnlFileType.pnlOptions.grpBMPOptions.hide();
    dlgMain.pnlFileType.pnlOptions.grpPngOptions.hide();
}


///////////////////////////////////////////////////////////////////////////////
// Function: initExportInfo
// Usage: create our default parameters
// Input: a new Object
// Return: a new object with params set to default
///////////////////////////////////////////////////////////////////////////////
function initExportInfo(exportInfo) {
    exportInfo.destination = new String("");
    exportInfo.fileNamePrefix = new String("untitled_");
    exportInfo.visibleOnly = true;
    exportInfo.fileType = pngIndex;
    exportInfo.icc = true;
    exportInfo.jpegQuality = 8;
    exportInfo.psdMaxComp = true;
    exportInfo.tiffCompression = TIFFEncoding.NONE;
    exportInfo.tiffJpegQuality = 8;
    exportInfo.pdfEncoding = PDFEncoding.JPEG;
    exportInfo.pdfJpegQuality = 8;
    exportInfo.targaDepth = TargaBitsPerPixels.TWENTYFOUR;
    exportInfo.bmpDepth = BMPDepthType.TWENTYFOUR;
    exportInfo.jx = false;

    try {
        exportInfo.destination = Folder(app.activeDocument.fullName.parent).fsName; // destination folder
        var tmp = app.activeDocument.fullName.name;
        exportInfo.fileNamePrefix = decodeURI(tmp.substring(0, tmp.indexOf("."))); // filename body part
    } catch(someError) {
        exportInfo.destination = new String("");
        exportInfo.fileNamePrefix = app.activeDocument.name; // filename body part
    }
}


///////////////////////////////////////////////////////////////////////////////
// Function: saveFile
// Usage: the worker routine, take our params and save the file accordingly
// Input: reference to the document, the name of the output file, 
//        export info object containing more information
// Return: <none>, a file on disk
///////////////////////////////////////////////////////////////////////////////
function saveFile( docRef, fileNameBody, exportInfo) {
    switch (exportInfo.fileType) {
        case jpegIndex:
            var saveFile = new File(exportInfo.destination + "/" + fileNameBody + ".jpg");
            jpgSaveOptions = new JPEGSaveOptions();
            jpgSaveOptions.embedColorProfile = exportInfo.icc;
            jpgSaveOptions.quality = exportInfo.jpegQuality;
            docRef.saveAs(saveFile, jpgSaveOptions, true, Extension.LOWERCASE);
            break;
        case psdIndex:
            var saveFile = new File(exportInfo.destination + "/" + fileNameBody + ".psd");
            psdSaveOptions = new PhotoshopSaveOptions();
            psdSaveOptions.embedColorProfile = exportInfo.icc;
            psdSaveOptions.maximizeCompatibility = exportInfo.psdMaxComp;
            docRef.saveAs(saveFile, psdSaveOptions, true, Extension.LOWERCASE);
            break;
        case tiffIndex:
            var saveFile = new File(exportInfo.destination + "/" + fileNameBody + ".tif");
            tiffSaveOptions = new TiffSaveOptions();
            tiffSaveOptions.embedColorProfile = exportInfo.icc;
            tiffSaveOptions.imageCompression = exportInfo.tiffCompression;
            if (TIFFEncoding.JPEG == exportInfo.tiffCompression) {
				tiffSaveOptions.jpegQuality = exportInfo.tiffJpegQuality;
			}
            docRef.saveAs(saveFile, tiffSaveOptions, true, Extension.LOWERCASE);
            break;
        case pdfIndex:
            var saveFile = new File(exportInfo.destination + "/" + fileNameBody + ".pdf");
            pdfSaveOptions = new PDFSaveOptions();
            pdfSaveOptions.embedColorProfile = exportInfo.icc;
            pdfSaveOptions.encoding = exportInfo.pdfEncoding;
            if (PDFEncoding.JPEG == exportInfo.pdfEncoding) {
				pdfSaveOptions.jpegQuality = exportInfo.pdfJpegQuality;
			}
            docRef.saveAs(saveFile, pdfSaveOptions, true, Extension.LOWERCASE);
            break;
        case targaIndex:
            var saveFile = new File(exportInfo.destination + "/" + fileNameBody + ".tga");
            targaSaveOptions = new TargaSaveOptions();
            targaSaveOptions.resolution = exportInfo.targaDepth;
            docRef.saveAs(saveFile, targaSaveOptions, true, Extension.LOWERCASE);
            break;
        case bmpIndex:
            var saveFile = new File(exportInfo.destination + "/" + fileNameBody + ".bmp");
            bmpSaveOptions = new BMPSaveOptions();
            bmpSaveOptions.depth = exportInfo.bmpDepth;
            docRef.saveAs(saveFile, bmpSaveOptions, true, Extension.LOWERCASE);
            break;
        case pngIndex:
            var saveFile = new File(exportInfo.destination + "/" + fileNameBody + ".png");
            pngSaveOptions = new PNGSaveOptions();
            pngSaveOptions.resolution = exportInfo.jx;
            docRef.saveAs(saveFile, pngSaveOptions, true, Extension.LOWERCASE);
            break;
        default:
            alert(strUnexpectedError);
            break;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Function: zeroSuppress
// Usage: return a string padded to digit(s)
// Input: num to convert, digit count needed
// Return: string padded to digit length
///////////////////////////////////////////////////////////////////////////////
function zeroSuppress (num, digit) {
    var tmp = num.toString();
    while (tmp.length < digit) {
		tmp = "0" + tmp;
	}
    return tmp;
}


///////////////////////////////////////////////////////////////////////////////
// Function: setInvisibleAllArtLayers
// Usage: unlock and make invisible all art layers, recursively
// Input: document or layerset
// Return: all art layers are unlocked and invisible
///////////////////////////////////////////////////////////////////////////////
function setInvisibleAllArtLayers(obj) {
    for( var i = 0; i < obj.artLayers.length; i++) {
        obj.artLayers[i].allLocked = false;
        obj.artLayers[i].visible = false;
    }
    for( var i = 0; i < obj.layerSets.length; i++) {
        setInvisibleAllArtLayers(obj.layerSets[i]);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Function: removeAllInvisibleArtLayers
// Usage: remove all the invisible art layers, recursively
// Input: document or layer set
// Return: <none>, all layers that were invisible are now gone
///////////////////////////////////////////////////////////////////////////////
function removeAllInvisibleArtLayers(obj) {
    for( var i = obj.artLayers.length-1; 0 <= i; i--) {
        try {
            if(!obj.artLayers[i].visible) {
				obj.artLayers[i].remove();
			}
        } 
        catch (e) {
        }
    }
    for( var i = obj.layerSets.length-1; 0 <= i; i--) {
        removeAllInvisibleArtLayers(obj.layerSets[i]);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Function: removeAllEmptyLayerSets
// Usage: find all empty layer sets and remove them, recursively
// Input: document or layer set
// Return: empty layer sets are now gone
///////////////////////////////////////////////////////////////////////////////
function removeAllEmptyLayerSets(obj) {
    var foundEmpty = true;
    for( var i = obj.layerSets.length-1; 0 <= i; i--) {
        if( removeAllEmptyLayerSets(obj.layerSets[i])) {
            obj.layerSets[i].remove();
        } else {
            foundEmpty = false;
        }
    }
    if (obj.artLayers.length > 0) {
		foundEmpty = false;
	}
    return foundEmpty;
}


///////////////////////////////////////////////////////////////////////////////
// Function: zeroSuppress
// Usage: return a string padded to digit(s)
// Input: num to convert, digit count needed
// Return: string padded to digit length
///////////////////////////////////////////////////////////////////////////////
function removeAllInvisible(docRef) {
    removeAllInvisibleArtLayers(docRef);
    removeAllEmptyLayerSets(docRef);
}

// 新建立文档
function newdoc()
{
	var desc1 = new ActionDescriptor();
	var desc2 = new ActionDescriptor();
	desc2.putString(app.stringIDToTypeID('preset'), "剪贴板");
	desc1.putObject(app.charIDToTypeID('Nw  '), app.charIDToTypeID('Dcmn'), desc2);
	executeAction(app.stringIDToTypeID('make'), desc1, DialogModes.NO);
	return app.activeDocument;
};


///////////////////////////////////////////////////////////////////////////////
// Function: exportChildren
// Usage: find all the children in this document to save
// Input: duplicate document, original document, export info,
//        reference to document, starting file name
// Return: <none>, documents are saved accordingly
///////////////////////////////////////////////////////////////////////////////
function exportChildren(dupObj, orgObj, exportInfo, dupDocRef, fileNamePrefix) {
    for( var i = 0; i < dupObj.artLayers.length; i++)
    {
        if (exportInfo.visibleOnly) { // visible layer only
            if (!orgObj.artLayers[i].visible) {
				continue;
			}
        }
        //dupObj.artLayers[i].visible = true;
        dupDocRef.activeLayer = dupObj.artLayers[i];
        
        dupDocRef.selection.selectAll();
		try
		{
			dupDocRef.selection.copy();
		}
		catch(someError)
		{
			alert("空图层!!!");
			continue;
		}

		dupDocRef.selection.deselect();

        var layerName = dupObj.artLayers[i].name;  // store layer name before change doc

        var duppedDocumentTmp = newdoc();
        duppedDocumentTmp.paste();
//        if (psdIndex == exportInfo.fileType) { // PSD: Keep transparency
//            removeAllInvisible(duppedDocumentTmp);
//        } else { // just flatten
//            duppedDocumentTmp.flatten();
//        }
/*
        var fileNameBody = fileNamePrefix;
        fileNameBody += "_" + zeroSuppress(i, 4);
        fileNameBody += "_" + layerName;
        fileNameBody = fileNameBody.replace(/[:\/\\*\?\"\<\>\|]/g, "_");  // '/\:*?"<>|' -> '_'
        if (fileNameBody.length > 120) {
			fileNameBody = fileNameBody.substring(0,120);
		}
*/
        var fileNameBody = "建筑";
        fileNameBody += "_" + zeroSuppress(i, 4);

		var xx = parseInt(dupDocRef.activeLayer.bounds[0],10);//eval(dupDocRef.activeLayer.bounds[0])
		var yy = parseInt(dupDocRef.activeLayer.bounds[1],10);
		logfile.write( fileNameBody + "," + xx + "," + yy + "\n" );

        saveFile(duppedDocumentTmp, fileNameBody, exportInfo);
        totle_num++;
        duppedDocumentTmp.close(SaveOptions.DONOTSAVECHANGES);

//        dupObj.artLayers[i].visible = false;
    }
    // 保存组里面的层
    for( var i = 0; i < dupObj.layerSets.length; i++) {
        if (exportInfo.visibleOnly) { // visible layer only
            if (!orgObj.layerSets[i].visible) {
				continue;
			}
        }
        var fileNameBody = fileNamePrefix;
        fileNameBody += "_" + zeroSuppress(i, 4) + "s";
        exportChildren(dupObj.layerSets[i], orgObj.layerSets[i], exportInfo, dupDocRef, fileNameBody);  // recursive call
    }
}
// End Export Layers To Files.js
