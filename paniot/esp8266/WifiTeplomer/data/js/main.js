var uri = "http://" + location.hostname + "/";
//var uri = "http://192.168.1.109/";

window.addEventListener("load", function load(event){
    window.removeEventListener("load", load, false);
    getTemperature();
    automaticallyGetTemperature();
    saveSetings();
},false);

function automaticallyGetTemperature() {
  setInterval(function(){ getTemperature() }, 10000);
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

function saveSetings() {
  $("form").on("submit", function (e) {
    e.preventDefault();
    $.ajax({
      type: 'POST',
      url: uri+'saveSetings',
      data: {
        'measurementInterval': $('#measurementInterval').val(),
        'elasticUrl': $('#elasticUrl').val(),
        'nameThermometer': $('#nameThermometer').val()
      },
      success: function(msg){
        alert('Nastaveni ulozeno');
      },
      timeout: 10000
    });
  });
}
