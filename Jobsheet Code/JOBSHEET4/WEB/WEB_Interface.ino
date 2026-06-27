// WEB INTERFACE
String AmbilHalamanWeb() {
  String html = "<!DOCTYPE html><html lang=\"id\"><head>";
  html += "<meta charset=\"UTF-8\">";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "<title>Trainer Presensi RFID</title>";
  html += "<style>";
  html += "*{margin:0;padding:0;box-sizing:border-box}";
  html += "body{font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;background:#f0f2f5;padding:20px}";
  html += ".container{max-width:1600px;margin:0 auto;background:white;padding:25px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1)}";
  html += "h1{text-align:center;color:#2c3e50;font-size:28px;margin-bottom:5px;letter-spacing:2px}";
  html += ".subtitle{text-align:center;color:#7f8c8d;font-size:14px;margin-bottom:20px}";
  html += ".last-tap-container{background:#e8f4fd;border-radius:8px;padding:10px 20px;margin-bottom:20px}";
  html += ".last-tap-item{padding:4px 0;font-size:14px;color:#2c3e50}";
  html += ".last-tap-item .name{font-weight:bold;color:#2980b9}";
  html += ".last-tap-item .time{color:#7f8c8d;font-size:13px;margin-left:10px}";
  html += ".last-tap-empty{color:#95a5a6;font-size:13px;font-style:italic;text-align:center;padding:4px 0}";
  html += ".toolbar{display:flex;justify-content:space-between;flex-wrap:wrap;gap:10px;margin-bottom:15px}";
  html += ".btn-group{display:flex;gap:10px;flex-wrap:wrap}";
  html += ".btn{padding:10px 20px;border:none;border-radius:5px;color:white;font-weight:bold;cursor:pointer;font-size:13px;transition:opacity .2s}";
  html += ".btn:hover{opacity:.85}";
  html += ".btn-excel{background:#27ae60}";
  html += ".btn-pdf{background:#e74c3c}";
  html += ".btn-clear{background:#7f8c8d}";
  html += ".status-badge{background:#3498db;color:white;padding:5px 15px;border-radius:20px;font-size:13px;display:inline-block}";
  html += ".month-table-wrapper{overflow-x:auto;margin-top:20px}";
  html += ".month-table-wrapper h2{background:#2c3e50;color:white;padding:8px 15px;border-radius:5px 5px 0 0;font-size:16px;margin:0}";
  html += ".month-table{width:100%;border-collapse:collapse;font-size:12px}";
  html += ".month-table th{background:#34495e;color:white;padding:6px 4px;text-align:center;border:1px solid #2c3e50;font-weight:bold}";
  html += ".month-table td{padding:4px 3px;text-align:center;border:1px solid #ddd;color:inherit}";
  html += ".sunday-cell{background-color:#ffcccc!important}";
  html += ".sunday-header{background-color:#ff6666!important;color:white!important;font-size:8px}";
  html += ".total-cell{background-color:#f0f0f0;font-weight:bold;color:#2c3e50}";
  html += ".total-header{background-color:#2c3e50;color:white;font-weight:bold}";
  html += ".month-table tbody tr:nth-child(even){background:#f8f9fa}";
  html += ".month-table tbody tr:hover{background:#d4edda!important;cursor:pointer}";
  html += ".col-no{width:35px;font-weight:bold;color:#2c3e50;font-size:11px}";
  html += ".col-uid{width:100px;font-family:monospace;color:#555;font-size:11px}";
  html += ".col-name{text-align:left;padding-left:8px;font-weight:600;color:#2c3e50;font-size:12px}";
  html += ".attendance-cell{width:32px;height:26px;text-align:center}";
  html += ".attendance-empty{color:#ddd;font-size:12px}";
  html += ".week-header{background:#d5dbdb;color:#2c3e50;font-size:10px}";
  html += ".day-header{background:#ecf0f1;color:#2c3e50;font-size:9px;padding:2px}";
  html += ".date-header{background:#f0f3f4;color:#7f8c8d;font-size:8px;padding:1px}";
  html += "@media(max-width:768px){.container{padding:10px}.month-table{font-size:10px}.col-uid{width:70px}.attendance-cell{width:24px}.toolbar{flex-direction:column;align-items:stretch}.btn-group{justify-content:center}.btn{flex:1;text-align:center}}";
  html += "</style></head><body>";
  
  html += "<div class=\"container\">";
  html += "<h1 id=\"judul-tabel\">TRAINER PRESENSI RFID</h1>";
  html += "<p class=\"subtitle\" id=\"sub-judul\">Sistem Presensi dengan RFID</p>";
  
  html += "<div class=\"last-tap-container\">";
  html += "<div id=\"last-taps-display\"><div class=\"last-tap-empty\">Belum ada tap</div></div>";
  html += "</div>";
  
  html += "<div class=\"toolbar\">";
  html += "<div class=\"btn-group\">";
  html += "<button class=\"btn btn-excel\" onclick=\"downloadExcel()\">📥 Download Excel</button>";
  html += "<button class=\"btn btn-pdf\" onclick=\"downloadPDF()\">📄 Download PDF</button>";
  html += "<button class=\"btn btn-clear\" onclick=\"resetData()\">🗑️ Reset Data</button>";
  html += "</div>";
  html += "<div><span class=\"status-badge\" id=\"total-badge\">Loading...</span></div>";
  html += "</div>";
  
  html += "<div id=\"table-container\"><p style=\"text-align:center;padding:20px;color:#95a5a6;\">Memuat data...</p></div>";
  html += "</div>";

  html += "<script src=\"https://cdn.jsdelivr.net/npm/xlsx@0.18.5/dist/xlsx.full.min.js\"></script>";
  html += "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jspdf/2.5.1/jspdf.umd.min.js\"></script>";
  html += "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/jspdf-autotable/3.5.25/jspdf.plugin.autotable.min.js\"></script>";
  
  html += "<script>";
  
  html += "let MODE_CONFIG = { mode: 'full', startMonth: 0, endMonth: 11, judul: 'ATTENDANCE LIST 2026', tahun: 2026 };";
  html += "let configLoaded = false;";
  
  html += "function loadConfig(){";
  html += "fetch('/getconfig')";
  html += ".then(function(r){return r.json();})";
  html += ".then(function(config){";
  html += "MODE_CONFIG = config;";
  html += "configLoaded = true;";
  html += "console.log('✅ Konfigurasi dari ESP32:', MODE_CONFIG);";
  html += "document.getElementById('judul-tabel').innerText = config.judul;";
  html += "document.getElementById('sub-judul').innerText = 'Sistem Presensi dengan RFID | ' + config.judul;";
  html += "updateData();";
  html += "})";
  html += ".catch(function(err){";
  html += "console.warn('⚠️ Gagal ambil konfigurasi, pakai default:', err);";
  html += "configLoaded = true;";
  html += "updateData();";
  html += "});";
  html += "}";
  
// DATABASE NAMA
  html += "const siswaDatabase=[";
  html += "{uid:'96906FB8',nama:'BAHLUL'},";
  html += "{uid:'991E7C12',nama:'RELAY 1CH 5VOLT MINI'},";
  html += "{uid:'058931B81D1300',nama:'SIMPLICITY'},";
  html += "{uid:'72A6FF0A',nama:'PARAGON'},";
  html += "{uid:'D287020B',nama:'DIMAS'},";
  html += "{uid:'040C559A687780',nama:'DIMSUM'}";
  html += "];";
  
  // HARI LIBUR NASIONAL
  html += "const holidayDates={";
  html += "'01/01/2026':true,'19/01/2026':true,'20/03/2026':true,";
  html += "'21/03/2026':true,'01/04/2026':true,'02/04/2026':true,";
  html += "'01/05/2026':true,'14/05/2026':true,'16/05/2026':true,";
  html += "'29/05/2026':true,'06/06/2026':true,'17/06/2026':true,";
  html += "'17/08/2026':true,'25/12/2026':true";
  html += "};";
  
  html += "const dayNames=['MON','TUE','WED','THU','FRI','SAT','SUN'];";
  html += "const dayNamesShort=['MO','TU','WE','TH','FR','SA','SU'];";
  html += "const monthNames=['JANUARY','FEBRUARY','MARCH','APRIL','MAY','JUNE','JULY','AUGUST','SEPTEMBER','OCTOBER','NOVEMBER','DECEMBER'];";
  
  html += "function getMonthName(m){return monthNames[m]||'JANUARY';}";
  html += "function getDayName(d){return dayNames[d-1]||'---';}";
  html += "function getDayNameShort(d){return dayNamesShort[d-1]||'--';}";
  html += "function isHoliday(d){return holidayDates[d]||false;}";
  
// DATA BULAN
  html += "function generateMonthData(year,month){";
  html += "var daysInMonth=new Date(year,month+1,0).getDate();";
  html += "var weeks=[];var dayCounter=1;";
  html += "for(var weekIndex=0;weekIndex<5;weekIndex++){";
  html += "var currentWeek=[];";
  html += "for(var d=0;d<7;d++){";
  html += "if(dayCounter<=daysInMonth){";
  html += "var dateObj=new Date(year,month,dayCounter);";
  html += "var dayOfWeek=dateObj.getDay()||7;";
  html += "var dateStr=('0'+dayCounter).slice(-2)+'/'+('0'+(month+1)).slice(-2)+'/'+year;";
  html += "var isRed=(dayOfWeek===7)||isHoliday(dateStr);";
  html += "currentWeek.push({day:dayCounter,dayName:getDayName(dayOfWeek),dayNameShort:getDayNameShort(dayOfWeek),dateStr:dateStr,isRed:isRed,isEmpty:false});";
  html += "dayCounter++;";
  html += "}else{";
  html += "currentWeek.push({day:'',dayName:'',dayNameShort:'',dateStr:'',isRed:false,isEmpty:true});";
  html += "}";
  html += "}";
  html += "weeks.push({weekNumber:weekIndex+1,days:currentWeek});";
  html += "}";
  html += "return {month:month,monthName:getMonthName(month),weeks:weeks,totalDays:daysInMonth};";
  html += "}";
  
  html += "function generateAllMonths(){";
  html += "var year=MODE_CONFIG.tahun || 2026;";
  html += "var months=[];";
  html += "var startM=0;";
  html += "var endM=11;";
  html += "var currentMode = (MODE_CONFIG.mode || 'full').toLowerCase();";
  html += "if(currentMode === 'range'){";
  html += "  startM = MODE_CONFIG.startMonth !== undefined ? parseInt(MODE_CONFIG.startMonth) : 0;";
  html += "  endM = MODE_CONFIG.endMonth !== undefined ? parseInt(MODE_CONFIG.endMonth) : 5;";
  html += "}else{";
  html += "  startM = 0;";
  html += "  endM = 11;";
  html += "}";
  html += "for(var m=startM;m<=endM;m++){";
  html += "  months.push(generateMonthData(year,m));";
  html += "}";
  html += "return months;";
  html += "}";
  
// TABEL
  html += "let attendanceData=[];";
  html += "let lastTaps=[];";
  html += "let dataLoaded=false;";
  
  html += "function generateTables(data){";
  html += "try{";
  
  html += "var judul = MODE_CONFIG.judul || 'ATTENDANCE LIST ' + MODE_CONFIG.tahun;";
  html += "document.getElementById('judul-tabel').innerText = judul;";
  html += "document.getElementById('sub-judul').innerText = 'Sistem Presensi dengan RFID | ' + judul;";
  
  html += "var months=generateAllMonths();";
  html += "var absensiMap={};";
  html += "if(data&&data.length>0){";
  html += "for(var i=0;i<data.length;i++){";
  html += "var item=data[i];";
  html += "if(item&&item.uid&&item.tanggal){";
  html += "var key=(item.uid||'').trim().toUpperCase()+'|'+(item.tanggal||'').trim();";
  html += "absensiMap[key]={jam:item.jam||'',week:item.week||''};";
  html += "}";
  html += "}";
  html += "}";
  
  html += "document.getElementById('total-badge').innerText=(data?data.length:0)+' Data Absensi';";
  html += "var allHtml='';";
  
  html += "for(var mi=0;mi<months.length;mi++){";
  html += "var monthData=months[mi];";
  html += "if(!monthData||!monthData.weeks||monthData.weeks.length===0){continue;}";
  html += "var weeks=monthData.weeks;";
  html += "var totalCols=3+(7*5)+1;";
  
  html += "allHtml+='<div class=\"month-table-wrapper\">';";
  html += "allHtml+='<h2>'+monthData.monthName+' '+MODE_CONFIG.tahun+'</h2>';";
  html += "allHtml+='<table class=\"month-table\">';";
  html += "allHtml+='<thead>';";
  
  html += "allHtml+='<tr><th colspan=\"'+totalCols+'\" style=\"background:#2c3e50;color:white;font-size:14px;padding:8px;text-align:center;\">&nbsp;</th></tr>';";
  
  html += "allHtml+='<tr>';";
  html += "allHtml+='<th style=\"background:#34495e;color:white;width:35px;\">No.</th>';";
  html += "allHtml+='<th style=\"background:#34495e;color:white;width:100px;\">ID NUMBER</th>';";
  html += "allHtml+='<th style=\"background:#34495e;color:white;text-align:left;padding-left:8px;\">NAME</th>';";
  html += "for(var w=0;w<weeks.length;w++){";
  html += "allHtml+='<th colspan=\"7\" class=\"week-header\">WEEK '+(w+1)+'</th>';";
  html += "}";
  html += "allHtml+='<th class=\"total-header\">TOTAL</th>';";
  html += "allHtml+='</tr>';";
  
  html += "allHtml+='<tr>';";
  html += "allHtml+='<th style=\"background:#34495e;\"></th>';";
  html += "allHtml+='<th style=\"background:#34495e;\"></th>';";
  html += "allHtml+='<th style=\"background:#34495e;\"></th>';";
  html += "for(var w=0;w<weeks.length;w++){";
  html += "var week=weeks[w];";
  html += "for(var d=0;d<7;d++){";
  html += "var day=week.days[d]||{isEmpty:true};";
  html += "var isRed=day.isRed||false;";
  html += "var dayName=day.isEmpty?'':(day.dayName||'');";
  html += "var redClass=isRed?'sunday-header':'day-header';";
  html += "allHtml+='<th class=\"'+redClass+'\">'+dayName+'</th>';";
  html += "}";
  html += "}";
  html += "allHtml+='<th class=\"total-header\">HADIR / HARI</th>';";
  html += "allHtml+='</tr>';";
  
  html += "allHtml+='<tr>';";
  html += "allHtml+='<th style=\"background:#34495e;\"></th>';";
  html += "allHtml+='<th style=\"background:#34495e;\"></th>';";
  html += "allHtml+='<th style=\"background:#34495e;\"></th>';";
  html += "for(var w=0;w<weeks.length;w++){";
  html += "var week=weeks[w];";
  html += "for(var d=0;d<7;d++){";
  html += "var day=week.days[d]||{isEmpty:true};";
  html += "var isRed=day.isRed||false;";
  html += "var dateValue=day.isEmpty?'':(('0'+day.day).slice(-2)||'');";
  html += "var redClass=isRed?'sunday-header':'date-header';";
  html += "allHtml+='<th class=\"'+redClass+'\">'+dateValue+'</th>';";
  html += "}";
  html += "}";
  html += "allHtml+='<th class=\"total-header\">'+monthData.totalDays+'</th>';";
  html += "allHtml+='</tr>';";
  
  html += "allHtml+='</thead><tbody>';";
  
  html += "var no=1;";
  html += "for(var si=0;si<siswaDatabase.length;si++){";
  html += "var siswa=siswaDatabase[si];";
  html += "if(!siswa||!siswa.uid)continue;";
  html += "allHtml+='<tr>';";
  html += "allHtml+='<td class=\"col-no\">'+(no++)+'</td>';";
  html += "allHtml+='<td class=\"col-uid\">'+siswa.uid+'</td>';";
  html += "allHtml+='<td class=\"col-name\">'+siswa.nama+'</td>';";
  html += "var totalHadir=0,totalHariKerja=0;";
  
  html += "for(var w=0;w<weeks.length;w++){";
  html += "var week=weeks[w];";
  html += "for(var d=0;d<7;d++){";
  html += "var day=week.days[d]||{isEmpty:true};";
  html += "if(day.isEmpty){";
  html += "allHtml+='<td class=\"attendance-cell\" style=\"background:#f9f9f9;\"></td>';";
  html += "}else{";
  html += "var isRed=day.isRed||false;";
  html += "var key=(siswa.uid||'').trim().toUpperCase()+'|'+(day.dateStr||'').trim();";
  html += "var isPresent=absensiMap[key]?true:false;";
  html += "var redClass=isRed?' sunday-cell':'';";
  html += "allHtml+='<td class=\"attendance-cell'+redClass+'\">';";
  html += "if(isPresent){";
  html += "allHtml+='X';";
  html += "if(!isRed)totalHadir++;";
  html += "}else{";
  html += "allHtml+='<span class=\"attendance-empty\">-</span>';";
  html += "}";
  html += "allHtml+='</td>';";
  html += "if(!isRed)totalHariKerja++;";
  html += "}";
  html += "}";
  html += "}";
  
  html += "var persen=(totalHariKerja>0)?Math.round((totalHadir/totalHariKerja)*100):0;";
  html += "allHtml+='<td class=\"total-cell\">'+totalHadir+'/'+totalHariKerja+' ('+persen+'%)</td>';";
  html += "allHtml+='</tr>';";
  html += "}";
  
  html += "allHtml+='</tbody></table></div>';";
  html += "}";
  
  html += "document.getElementById('table-container').innerHTML=allHtml;";
  html += "dataLoaded=true;";
  html += "}catch(e){";
  html += "console.error('Error generateTables:',e);";
  html += "document.getElementById('table-container').innerHTML='<p style=\"text-align:center;padding:20px;color:red;\">Error: '+e.message+'</p>';";
  html += "}";
  html += "}";

  html += "function updateLastTaps(taps){";
  html += "try{";
  html += "var container=document.getElementById('last-taps-display');";
  html += "if(!taps||taps.length===0){";
  html += "container.innerHTML='<div class=\"last-tap-empty\">Belum ada tap</div>';return;";
  html += "}";
  html += "var html='';";
  html += "for(var i=0;i<taps.length;i++){";
  html += "var tap=taps[i];";
  html += "if(tap&&tap.nama){";
  html += "html+='<div class=\"last-tap-item\"><span class=\"name\">'+(tap.nama||'')+'</span><span class=\"time\">'+(tap.waktu||'')+'</span></div>';";
  html += "}";
  html += "}";
  html += "container.innerHTML=html||'<div class=\"last-tap-empty\">Belum ada tap</div>';";
  html += "}catch(e){console.error('Error updateLastTaps:',e);}";
  html += "}";

  html += "function updateData(){";
  html += "console.log('📡 Mengambil data dari ESP32...');";
  html += "fetch('/getdata', { cache: 'no-store' })";
  html += ".then(function(r){";
  html += "if(!r.ok){throw new Error('HTTP Error: '+r.status);}";
  html += "return r.text();";
  html += "})";
  html += ".then(function(textData){";
  html += "try {";
  html += "var res = JSON.parse(textData);";
  html += "console.log('✅ Data diterima:', res);";
  
  html += "if(res.last_taps){";
  html += "updateLastTaps(res.last_taps);";
  html += "}";
  
  html += "attendanceData = res.log_absensi || [];";
  
  html += "if(res.last_taps && res.last_taps.length > 0){";
  html += "for(var t=0; t<res.last_taps.length; t++){";
  html += "var tap = res.last_taps[t];";
  html += "if(tap && tap.waktu){";

  html += "var parts = tap.waktu.split(' | ');";
  html += "if(parts.length >= 2){";
  html += "var jamTap = parts[0];";
  html += "var tglTap = parts[1];";
  html += "var weekTap = parts[2] ? parts[2].replace('Week ', '') : '1';";
  
  html += "var siswaDitemukan = siswaDatabase.find(function(s){return s.nama === tap.nama;});";
  html += "if(siswaDitemukan){";

  html += "var sudahAda = attendanceData.some(function(a){";
  html += "return a.uid === siswaDitemukan.uid && a.tanggal === tglTap;";
  html += "});";

  html += "if(!sudahAda){";
  html += "attendanceData.push({";
  html += "uid: siswaDitemukan.uid,";
  html += "nama: siswaDitemukan.nama,";
  html += "tanggal: tglTap,";
  html += "jam: jamTap,";
  html += "week: weekTap";
  html += "});";
  html += "}";
  html += "}";
  html += "}";
  html += "}";
  html += "}";
  html += "}";
  
  html += "generateTables(attendanceData);";
  html += "} catch(jsonErr) {";
  html += "console.error('❌ JSON Error:', jsonErr, 'Data:', textData);";
  html += "}";
  html += "})";
  html += ".catch(function(err){";
  html += "console.error('❌ Koneksi Gagal:', err);";
  html += "if(!dataLoaded){";
  html += "document.getElementById('table-container').innerHTML = '<p style=\"text-align:center;padding:20px;color:red;\">⚠️ Gagal terhubung ke ESP32</p>';";
  html += "}";
  html += "});";
  html += "}";
  
// DOWNLOAD EXCEL
  html += "function downloadExcel(){";
  html += "loadScript('https://cdn.jsdelivr.net/npm/xlsx@0.18.5/dist/xlsx.full.min.js',function(){";
  html += "try{";
  html += "var months=generateAllMonths();";
  html += "var absMap={};";
  html += "if(attendanceData&&attendanceData.length>0){";
  html += "for(var i=0;i<attendanceData.length;i++){";
  html += "var item=attendanceData[i];";
  html += "if(item&&item.uid&&item.tanggal){";
  html += "absMap[(item.uid||'').trim().toUpperCase()+'|'+(item.tanggal||'').trim()]=true;";
  html += "}";
  html += "}";
  html += "}";
  
  html += "var wb=XLSX.utils.book_new();";
  html += "var allRows=[];";
  html += "var judulFile = MODE_CONFIG.judul || 'Attendance_List';";
  
  html += "for(var mi=0;mi<months.length;mi++){";
  html += "var md=months[mi];";
  html += "if(!md||!md.weeks)continue;";
  html += "if(mi>0){allRows.push([]);allRows.push([]);}";
  html += "var totalCols=3+(7*5)+1;";
  
  html += "var row1=[];var title=md.monthName+' '+MODE_CONFIG.tahun;";
  html += "for(var c=0;c<totalCols;c++){row1.push(c===0?title:'');}";
  html += "allRows.push(row1);";
  
  html += "var row2=['No.','ID NUMBER','NAME'];";
  html += "for(var wi=0;wi<md.weeks.length;wi++){";
  html += "for(var d=0;d<7;d++){row2.push('WEEK '+(wi+1));}";
  html += "}";
  html += "row2.push('TOTAL');";
  html += "allRows.push(row2);";
  
  html += "var row3=['','',''];";
  html += "for(var wi=0;wi<md.weeks.length;wi++){";
  html += "var week=md.weeks[wi];";
  html += "for(var d=0;d<7;d++){";
  html += "var day=week.days[d]||{isEmpty:true};";
  html += "row3.push(day.isEmpty?'':(day.dayName||''));";
  html += "}";
  html += "}";
  html += "row3.push('HADIR / HARI');";
  html += "allRows.push(row3);";
  
  html += "var row4=['','',''];";
  html += "for(var wi=0;wi<md.weeks.length;wi++){";
  html += "var week=md.weeks[wi];";
  html += "for(var d=0;d<7;d++){";
  html += "var day=week.days[d]||{isEmpty:true};";
  html += "row4.push(day.isEmpty?'':(('0'+day.day).slice(-2)||''));";
  html += "}";
  html += "}";
  html += "row4.push(md.totalDays);";
  html += "allRows.push(row4);";
  
  html += "var no=1;";
  html += "for(var si=0;si<siswaDatabase.length;si++){";
  html += "var siswa=siswaDatabase[si];";
  html += "if(!siswa||!siswa.uid)continue;";
  html += "var row=[no++,siswa.uid,siswa.nama];";
  html += "var totalHadir=0,totalHariKerja=0;";
  html += "for(var wi=0;wi<md.weeks.length;wi++){";
  html += "var week=md.weeks[wi];";
  html += "for(var d=0;d<7;d++){";
  html += "var day=week.days[d]||{isEmpty:true};";
  html += "if(day.isEmpty){row.push('');}else{";
  html += "var key=(siswa.uid||'').trim().toUpperCase()+'|'+(day.dateStr||'').trim();";
  html += "var isPresent=absMap[key]?'X':'';";
  html += "row.push(isPresent);";
  html += "if(isPresent==='X'&&!day.isRed)totalHadir++;";
  html += "if(!day.isRed)totalHariKerja++;";
  html += "}";
  html += "}";
  html += "}";
  html += "var persen=(totalHariKerja>0)?Math.round((totalHadir/totalHariKerja)*100):0;";
  html += "row.push(totalHadir+'/'+totalHariKerja+' ('+persen+'%)');";
  html += "allRows.push(row);";
  html += "}";
  html += "}";
  
  html += "var ws=XLSX.utils.aoa_to_sheet(allRows);";
  html += "ws['!merges']=[];";
  html += "ws['!cols']=[{wch:5},{wch:14},{wch:30}];";
  html += "for(var c=0;c<35;c++){ws['!cols'].push({wch:5});}";
  html += "ws['!cols'].push({wch:18});";
  
  html += "ws['!rows']=[];";
  html += "for(var r=0;r<allRows.length;r++){";
  html += "var isHeader=false;var tempRowIndex=0;";
  html += "for(var mi=0;mi<months.length;mi++){";
  html += "var md=months[mi];if(!md||!md.weeks)continue;";
  html += "var headerRows=4;var dataRows=siswaDatabase.length;var gapRows=(mi>0)?2:0;";
  html += "if(r>=tempRowIndex&&r<tempRowIndex+headerRows){isHeader=true;break;}";
  html += "tempRowIndex+=headerRows+dataRows+gapRows;";
  html += "}";
  html += "ws['!rows'].push({hpt:isHeader?22:16});";
  html += "}";
  
  html += "XLSX.utils.book_append_sheet(wb,ws,'Attendance');";
  html += "XLSX.writeFile(wb,judulFile.replace(/ /g,'_')+'.xlsx');";
  html += "}catch(e){alert('Error download Excel: '+e.message);console.error(e);}";
  html += "});";
  html += "}";
  
// DOWNLOAD PDF
  html += "function downloadPDF(){";
  html += "loadScript('https://cdnjs.cloudflare.com/ajax/libs/jspdf/2.5.1/jspdf.umd.min.js',function(){";
  html += "loadScript('https://cdnjs.cloudflare.com/ajax/libs/jspdf-autotable/3.5.25/jspdf.plugin.autotable.min.js',function(){";
  html += "try{";
  html += "const{jsPDF}=window.jspdf;";
  html += "var doc=new jsPDF('l','mm','a4');";
  html += "var startY=15, bulanDiHalaman=0;";
  html += "var judulFile = MODE_CONFIG.judul || 'ATTENDANCE LIST ' + MODE_CONFIG.tahun;";
  html += "doc.setFontSize(16);doc.text(judulFile,148,startY,{align:'center'});startY+=10;";
  html += "var months=generateAllMonths();var absMap={};";
  html += "if(attendanceData&&attendanceData.length>0){";
  html += "for(var i=0;i<attendanceData.length;i++){";
  html += "var item=attendanceData[i];";
  html += "if(item&&item.uid&&item.tanggal){";
  html += "absMap[(item.uid||'').trim().toUpperCase()+'|'+(item.tanggal||'').trim()]=true;";
  html += "}";
  html += "}";
  html += "}";
  
  html += "for(var mi=0;mi<months.length;mi++){";
  html += "var md=months[mi];";
  html += "if(!md||!md.weeks)continue;";
  html += "if(mi>0&&(bulanDiHalaman>=3||startY>200)){doc.addPage();startY=20;bulanDiHalaman=0;}";
  
  html += "var tableData=[];";
  html += "var header=['No.','ID NUMBER','NAME'];";
  html += "for(var wi=0;wi<md.weeks.length;wi++){";
  html += "var week=md.weeks[wi];";
  html += "for(var di=0;di<7;di++){";
  html += "var day=week.days[di]||{isEmpty:true};";
  html += "header.push(day.isEmpty?'':(day.dayNameShort||''));";
  html += "}";
  html += "}";
  html += "header.push('TOTAL');tableData.push(header);";
  
  html += "var dateHeader=['','',''];";
  html += "for(var wi=0;wi<md.weeks.length;wi++){";
  html += "var week=md.weeks[wi];";
  html += "for(var di=0;di<7;di++){";
  html += "var day=week.days[di]||{isEmpty:true};";
  html += "dateHeader.push(day.isEmpty?'':(('0'+day.day).slice(-2)||''));";
  html += "}";
  html += "}";
  html += "dateHeader.push('HARI KERJA');tableData.push(dateHeader);";
  
  html += "var no=1;";
  html += "for(var si=0;si<siswaDatabase.length;si++){";
  html += "var siswa=siswaDatabase[si];";
  html += "if(!siswa||!siswa.uid)continue;";
  html += "var row=[no++,siswa.uid,siswa.nama];";
  html += "var totalHadir=0,totalHariKerja=0;";
  html += "for(var wi=0;wi<md.weeks.length;wi++){";
  html += "var week=md.weeks[wi];";
  html += "for(var di=0;di<7;di++){";
  html += "var day=week.days[di]||{isEmpty:true};";
  html += "if(day.isEmpty){row.push('');}else{";
  html += "var key=(siswa.uid||'').trim().toUpperCase()+'|'+(day.dateStr||'').trim();";
  html += "var isPresent=absMap[key]?'X':'';";
  html += "row.push(isPresent);";
  html += "if(isPresent==='X'&&!day.isRed)totalHadir++;";
  html += "if(!day.isRed)totalHariKerja++;";
  html += "}";
  html += "}";
  html += "}";
  html += "var persen=(totalHariKerja>0)?Math.round((totalHadir/totalHariKerja)*100):0;";
  html += "row.push(totalHadir+'/'+totalHariKerja+' ('+persen+'%)');";
  html += "tableData.push(row);";
  html += "}";
  
  html += "doc.setFontSize(11);";
  html += "doc.text(md.monthName+' '+MODE_CONFIG.tahun,148,startY,{align:'center'});startY+=6;";
  html += "doc.autoTable({";
  html += "head:[tableData[0]],";
  html += "body:tableData.slice(2),";
  html += "foot:[tableData[1]],";
  html += "startY:startY,theme:'grid',styles:{fontSize:6,cellPadding:1},";
  html += "headStyles:{fillColor:[44,62,80],textColor:[255,255,255],fontSize:6,fontStyle:'bold'},";
  html += "footStyles:{fillColor:[200,200,200],textColor:[0,0,0],fontSize:6},";
  html += "columnStyles:{0:{cellWidth:6},1:{cellWidth:20},2:{cellWidth:28}}";
  html += "});";
  
  html += "startY=doc.lastAutoTable.finalY+5;bulanDiHalaman++;";
  html += "}";
  
  html += "doc.save(judulFile.replace(/ /g,'_')+'.pdf');";
  html += "}catch(e){alert('Error download PDF: '+e.message);}";
  html += "});";
  html += "});";
  html += "}";
  
// RESET DATA & LOAD SCRIPT
  html += "function resetData(){";
  html += "if(confirm('Hapus semua data absensi?')){";
  html += "fetch('/clear').then(function(){";
  html += "attendanceData=[];";
  html += "document.getElementById('total-badge').innerText='0 Data Absensi';";
  html += "generateTables(attendanceData);";
  html += "});";
  html += "}";
  html += "}";
  
  html += "function loadScript(url,callback){";
  html += "var script=document.createElement('script');";
  html += "script.src=url;script.onload=callback;";
  html += "script.onerror=function(){alert('Gagal load: '+url);};";
  html += "document.head.appendChild(script);";
  html += "}";
  // 
  html += "window.onload=function(){";
  html += "loadConfig();";
  html += "};";
  
  html += "setInterval(updateData,5000);";
  html += "</script></body></html>";
  
  return html;
}