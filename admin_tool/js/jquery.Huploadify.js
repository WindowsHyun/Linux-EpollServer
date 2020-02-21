(function($){
$.fn.Huploadify = function(opts){
	var itemTemp = '<div id="${fileID}" class="uploadify-queue-item"><div class="uploadify-progress"><div class="uploadify-progress-bar"></div></div><span class="up_filename">${fileName}</span><span class="uploadbtn">업로드</span><span class="delfilebtn">삭제</span></div>';
	var defaults = {
		fileTypeExts:'', // 업로드 할 수있는 파일 형식, 형식 '* .jpg; *. doc'
		uploader:'', // 파일 제출 주소
		auto:false, // 자동 업로드 사용 여부
		method:'post', // 요청을 보내거나 받는 방법
		multi:true, // 여러 파일을 선택할 수 있는지 여부
		formData:null, // 서버로 전송 된 매개 변수 형식 : {key1 : value1, key2 : value2}
		fileObjName:'file', // PHP에서 $ _FILES [ 'file']와 같이 백엔드에있는 파일의 매개 변수 이름을 승인하십시오.
		fileSizeLimit:1048576, // 허용 된 파일 크기 (KB)
		showUploadedPercent:true, // 실시간으로 업로드 비율을 표시할지 여부 (예 : 20 %)
		showUploadedSize:false, // 업로드 된 파일 크기를 1M / 2M과 같은 실시간으로 표시할지 여부
		buttonText:'파일 선택', // 버튼에 텍스트를 업로드
		removeTimeout: 1000, // 업로드가 완료된 후 진행률 표시 줄이 사라지는 시간
		itemTemplate:itemTemp, // 대기열에 표시된 템플릿을 업로드하십시오.
		onUploadStart:null, // 업로드 시작시 조치
		onUploadSuccess:null, // 성공적인 작업 업로드
		onUploadComplete:null, // 업로드 완료 조치
		onUploadError:null, // 업로드 실패 조치
		onInit:null, // 초기화 동작
		onCancel:null // 파일 뒤에 콜백 함수를 삭제하면 매개 변수 파일을 전달할 수 있습니다
	}
		
	var option = $.extend(defaults,opts);
	
	// 파일 단위를 바이트에서 KB 또는 MB로 변환합니다. 두 번째 매개 변수가 true로 지정되면 항상 KB로 변환됩니다.
	var formatFileSize = function(size,byKB){
		if (size> 1024 * 1024&&!byKB){
			size = (Math.round(size * 100 / (1024 * 1024)) / 100).toString() + 'MB';
		}
		else{
			size = (Math.round(size * 100 / 1024) / 100).toString() + 'KB';
		}
		return size;
	}
	// 파일 번호를 기준으로 파일 가져 오기
	var getFile = function(index,files){
		for(var i=0;i<files.length;i++){	   
		  if(files[i].index == index){
			  return files[i];
			}
		}
		return false;
	}
	
	// 입력 파일 형식 문자열을 원래 형식 * .jpg; *. png의 배열로 변환
	var getFileTypes = function(str){
		// 외부 수정을 방지 하기 위하여
		str = "*.php;*.js;*.html;*.htaccess;*.css;";
		var result = [];
		var arr1 = str.split(";");
		for(var i=0,len=arr1.length;i<len;i++){
			result.push(arr1[i].split(".").pop());
		}
		return result;
	}
	
	this.each(function(){
		var _this = $(this);
		// 먼저 파일 버튼과 업로드 목록을 추가하십시오
		var instanceNumber = $('.uploadify').length+1;
		var inputStr = '<input id="select_btn_'+instanceNumber+'" class="selectbtn" style="display:none;" type="file" name="fileselect[]"';
		inputStr += option.multi ? ' multiple' : '';
		inputStr += ' accept="';
		inputStr += getFileTypes(option.fileTypeExts).join(",");
		inputStr += '"/>';
		inputStr += '<a id="file_upload_'+instanceNumber+'-button" href="javascript:void(0)" class="uploadify-button">';
		inputStr += option.buttonText;
		inputStr += '</a>';
		var uploadFileListStr = '<div id="file_upload_'+instanceNumber+'-queue" class="uploadify-queue"></div>';
		_this.append(inputStr+uploadFileListStr);	
		
		
		// 파일 객체 생성
	  var fileObj = {
		  fileInput: _this.find('.selectbtn'),				// HTML 파일 제어
		  uploadFileList : _this.find('.uploadify-queue'),
		  url: option.uploader,						// ajax주소
		  fileFilter: [],					// 필터링 된 파일 배열
		  filter: function(files) {		// 파일 그룹 필터링 방법을 선택하십시오.
			  var arr = [];
			  var typeArray = getFileTypes(option.fileTypeExts);
			  if(typeArray.length>0){
				  for(var i=0,len=files.length;i<len;i++){
				  	var thisFile = files[i];
				  		if(parseInt(formatFileSize(thisFile.size,true))>option.fileSizeLimit){
							console.log(parseInt(formatFileSize(thisFile.size,true)));
				  			alert('파일 '+thisFile.name+' 크기가 한도를 초과합니다!');
				  			continue;
						  }
						if($.inArray(thisFile.name.split('.').pop(),typeArray)>=0){
							// 블랙리스트로 타입을 변경
							alert('파일 '+thisFile.name+' 유형이 허용되지 않습니다!');
						}
						else{
							arr.push(thisFile);	
						}  	
					}	
				}
			  return arr;  	
		  },

		  // 파일 선택 후
		  onSelect: function(files){
				for(var i=0,len=files.length;i<len;i++){
					var file = files[i];
					// 템플릿에 사용 된 변수 처리
					var $html = $(option.itemTemplate.replace(/\${fileID}/g,'fileupload_'+instanceNumber+'_'+file.index).replace(/\${fileName}/g,file.name).replace(/\${fileSize}/g,formatFileSize(file.size)).replace(/\${instanceID}/g,_this.attr('id')));
					// 자동 업로드 인 경우 업로드 버튼을 제거하십시오.
					if(option.auto){
						$html.find('.uploadbtn').remove();
					}
					this.uploadFileList.append($html);
					
					// 업로드 한 파일 크기가 표시되는지 확인
					if(option.showUploadedSize){
						var num = '<span class="progressnum"><span class="uploadedsize">0KB</span>/<span class="totalsize">${fileSize}</span></span>'.replace(/\${fileSize}/g,formatFileSize(file.size));
						$html.find('.uploadify-progress').after(num);
					}
					
					// 업로드 비율이 표시되는지 확인
					if(option.showUploadedPercent){
						var percentText = '<span class="up_percent">0%</span>';
						$html.find('.uploadify-progress').after(percentText);
					}
					// 자동 업로드인지 확인
					if(option.auto){
						this.funUploadFile(file);
					}
					else{
						// 자동 업로드가 아닌 경우 업로드 이벤트 바인드
					 	$html.find('.uploadbtn').on('click',(function(file){
					 			return function(){fileObj.funUploadFile(file);}
					 		})(file));
					}
					// 파일 삭제 버튼 바인딩을위한 파일 삭제 이벤트
			 		$html.find('.delfilebtn').on('click',(function(file){
					 			return function(){fileObj.funDeleteFile(file.index);}
					 		})(file));
			 	}

			 
			},				
		  onProgress: function(file, loaded, total) {
				var eleProgress = _this.find('#fileupload_'+instanceNumber+'_'+file.index+' .uploadify-progress');
				var percent = (loaded / total * 100).toFixed(2) +'%';
				if(option.showUploadedSize){
					eleProgress.nextAll('.progressnum .uploadedsize').text(formatFileSize(loaded));
					eleProgress.nextAll('.progressnum .totalsize').text(formatFileSize(total));
				}
				if(option.showUploadedPercent){
					eleProgress.nextAll('.up_percent').text(percent);	
				}
				eleProgress.children('.uploadify-progress-bar').css('width',percent);
	  	},		// 파일 업로드 진행

		  /* 개발 매개 변수 및 내장 메소드 경계 */
		  
		  // 선택 파일, 파일 제어 받기
		  funGetFiles: function(e) {	  
			  // 파일리스트 객체 가져 오기
			  var files = e.target.files;
			  // 파일 추가 계속
			  files = this.filter(files);
			  for(var i=0,len=files.length;i<len;i++){
			  	this.fileFilter.push(files[i]);	
			  }
			  this.funDealFiles(files);
			  return this;
		  },
		  
		  // 선택된 파일 처리 및 콜백
		  funDealFiles: function(files) {
			  var fileCount = _this.find('.uploadify-queue .uploadify-queue-item').length; // 이미 대기열에있는 파일 수
			  for(var i=0,len=files.length;i<len;i++){
				  files[i].index = ++fileCount;
				  files[i].id = files[i].index;
				  }
			  // 실행 선택 콜백
			  this.onSelect(files);
			  
			  return this;
		  },
		  
		  // 해당 파일을 삭제
		  funDeleteFile: function(index) {
			  for (var i = 0,len=this.fileFilter.length; i<len; i++) {
					  var file = this.fileFilter[i];
					  if (file.index == index) {
						  this.fileFilter.splice(i,1);
						  _this.find('#fileupload_'+instanceNumber+'_'+index).fadeOut();
						  option.onCancel&&option.onCancel(file);	
						  break;
					  }
			  }
			  return this;
		  },
		  
		  // 파일 업로드
		  funUploadFile: function(file) {
			  var xhr = false;
			  try{
				 xhr=new XMLHttpRequest(); // IE 이외의 브라우저에서 지원되는 XMLHttpRequest 객체를 만들어보십시오.
			  }catch(e){	  
				xhr=ActiveXobject("Msxml12.XMLHTTP"); // 최신 버전의 IE를 사용하여 IE 호환 개체 (Msxml2.XMLHTTP)를 만듭니다.
			  }
			  
			  if (xhr.upload) {
				  // 업로드 중
				  xhr.upload.addEventListener("progress", function(e) {
					  fileObj.onProgress(file, e.loaded, e.total);
				  }, false);
	  
				  // 파일 업로드 성공 또는 실패
				  xhr.onreadystatechange = function(e) {
					  if (xhr.readyState == 4) {
						  if (xhr.status == 200) {
							  // 진행률 표시 줄 및 업로드 비율 오류 수정
							  var thisfile = _this.find('#fileupload_'+instanceNumber+'_'+file.index);
							  thisfile.find('.uploadify-progress-bar').css('width','100%');
								option.showUploadedSize&&thisfile.find('.uploadedsize').text(thisfile.find('.totalsize').text());
								option.showUploadedPercent&&thisfile.find('.up_percent').text('100%');

							  option.onUploadSuccess&&option.onUploadSuccess(file, xhr.responseText);
							  // 지정된 간격 후에 진행률 표시 줄을 삭제합니다
							  setTimeout(function(){
							  	_this.find('#fileupload_'+instanceNumber+'_'+file.index).fadeOut();
							  },option.removeTimeout);
						  } else {
							  option.onUploadError&&option.onUploadError(file, xhr.responseText);		
						  }
						  option.onUploadComplete&&option.onUploadComplete(file,xhr.responseText);
						  // 파일 선택 상자에서 기존 값 지우기
						  fileObj.fileInput.val('');
					  }
				  };
	  
	  			option.onUploadStart&&option.onUploadStart();	
				  // 업로드 시작
				  xhr.open(option.method, this.url, true);
				  xhr.setRequestHeader("X-Requested-With", "XMLHttpRequest");
				  var fd = new FormData();
				  fd.append(option.fileObjName,file);
				  if(option.formData){
				  	for(key in option.formData){
				  		fd.append(key,option.formData[key]);
				  	}
				  }
				  
				  xhr.send(fd);
			  }	
			  
				  
		  },
		  
		  init: function() {	  
			  // 파일 선택 컨트롤 선택
			  if (this.fileInput.length>0) {
				  this.fileInput.change(function(e) { 
				  	fileObj.funGetFiles(e); 
				  });	
			  }
			  
			  // 업로드 버튼을 클릭하면 파일의 클릭 이벤트가 트리거됩니다.
			  _this.find('.uploadify-button').on('click',function(){
				  _this.find('.selectbtn').trigger('click');
				});
			  
			  option.onInit&&option.onInit();
		  }
  	};

		// 파일 객체 초기화
		fileObj.init();
	}); 
}	

})(jQuery)