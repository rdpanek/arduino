var uri = "http://" + location.hostname + "/";
//var uri = "http://192.168.1.103/";

window.addEventListener("load", function load(event){
    window.removeEventListener("load", load, false);
    getTemperature();
    automaticallyGetTemperature();
    saveSetings();
    saveDisplaySetings();
},false);

function automaticallyGetTemperature() {
  setInterval(function(){ getTemperature() }, 10000);
  setInterval(function(){ getLdr() }, 10000);
}

function getTemperature() {
  $.ajax({
    url: uri+'getTemperature',
    success: function(result) {
      $("#div1").html(result);
      $( ".temperature-box" ).html(result);
    },
    timeout: 3000
  });
}

function getLdr() {
  $.ajax({
    url: uri+'getLdr',
    success: function(result) {
      $("#div1").html(result);
      $( ".ldr-box" ).html(result);
    },
    timeout: 3000
  });
}

function saveSetings() {
  $("#settings").on("submit", function (e) {
    e.preventDefault();
    $.ajax({
      type: 'POST',
      url: uri+'saveSetings',
      data: {
        'delayBetweenMeasurement': $('#delayBetweenMeasurement').val(),
        'elasticsearchUri': $('#elasticsearchUri').val(),
        'nazevZarizeni': $('#nazevZarizeni').val()
      },
      success: function(msg){
        alert('Nastaveni ulozeno');
      },
      timeout: 10000
    });
  });
}

function saveDisplaySetings() {
  $("#displaySettings").on("submit", function (e) {
    e.preventDefault();
    $.ajax({
      type: 'POST',
      url: uri+'saveDisplaySetings',
      data: {
        'displaySetup': $('#displaySetup').val(),
        'displayRotation': $('#displayRotation').val()
      },
      success: function(msg){
        alert('Nastaveni ulozeno');
      },
      timeout: 10000
    });
  });
}
