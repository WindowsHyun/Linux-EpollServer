$(document).ready(function() {
  // 보여지는 모든 table에 작업을 한다.
  // var table = $('table.display').DataTable({
  //     responsive: true,
  //     order: [
  //         [1, "asc"]
  //     ]
  // });
  $('#WakeOnLanDataTable').DataTable({
      responsive: true,
      order: [ [0, "asc"]]
  });

  $('#fileServerDataTable').DataTable({
    responsive: true,
    order: [ [1, "asc"]]
  });

  $('#adminDataTable').DataTable({
    responsive: true,
    order: [ [0, "asc"]]
  });

  $('#boardTable').DataTable({
    responsive: true,
    order: [ [0, "asc"]]
  });
    
});