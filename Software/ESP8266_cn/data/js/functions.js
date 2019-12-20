/*
$('[name="status"]').bootstrapSwitch({    
  onText:"on",    
  offText:"off",    
  onColor:"success",    
  offColor:"info",    
  size:"smail",    

})
  $('[name="Sniffer"]').bootstrapSwitch({    
  onText:"on",    
  offText:"off",    
  onColor:"success",    
  offColor:"info",    
  size:"smail",  
  state:"true",  

})

$('[name="attack"]').bootstrapSwitch({    
  onText:"on",    
  offText:"off",    
  onColor:"success",    
  offColor:"info",    
  size:"mini",     
 }
)*/

var i = 1;
var flag;

/*
$('[name="RF_attack"]').bootstrapSwitch({    
  onText:"on",    
  offText:"off",    
  onColor:"success",    
  offColor:"info",    
  size:"smail",     
  onSwitchChange:function(event,state){    
    if(state==true){    
       //alert('已打开'); 
       $('#RF_prog_1').show(); 
       $('#RF_Blast').hide(); 
       $("#RF_prog").css("width","0%");
       flag=setInterval(RF_attack_prog, 100);
       rf_blast_post();
    }else{    
       clearInterval(flag);
       $('#RF_prog_1').hide();
       $('#RF_Blast').show(); 
       $("#RF_prog").css("width","0%");
    }    
  }
 }
) 
*/

function rf_blast_post() {
    $.ajax({
        type: "POST",//方法类型
        dataType: "json",//预期服务器返回的数据类型
        url: "/rf_blast_c2",//url
        data: $('#rf_blast').serialize(),
        success: function (result) {
            console.log(result);//打印服务端返回的数据(调试用)
            if (result.resultCode == 200) {
                alert("SUCCESS");
            }
        },
    });
}


function RF_attack_prog() {
    /*  i++;
      $("#RF_prog").css("width",i+"%");
        if(i==100||i>100){
        clearInterval(flag);
        console.log("end");
        //$('[name="RF_attack"]').bootstrapSwitch('setState', false);
        $('[name="RF_attack"]').bootstrapSwitch('state',false);
        i=1;
      }
    */
    fetch('/ajax_rf_prog').then(function (res) {
        return res;
    }).then(function (res) {
        return res.text();
    }).then(function (data) {
        if (data) {

            //console.log(data);
            var prog = data;
            $("#RF_prog").css("width", prog + "%");
            if (prog == 100 || prog > 100) {
                clearInterval(flag);
                //console.log("end");
                //$('[name="RF_attack"]').bootstrapSwitch('setState', false);
                $('[name="RF_attack"]').bootstrapSwitch('state', false);
            }
        }

    })


}


$('#nfc_close').on('click', function () {
    $('#nfc_action').hide();
});

$('#rf_close').on('click', function () {
    $('#rf_action').hide();
});


$('#rf_close_24l01').on('click', function () {
    $('#rf_action_24l01').hide();
});


/*function render() {
    fetch('/ajax').then(function(res){
        return res;
    }).then(function(res){
        return res.text();
    }).then(function(data){
        document.getElementById("result").innerHTML = data;
    })
}*/

function operateFormatter(value, row, index) {
    return [
        '<button type="button" class="Play btn-xs btn-info" style="margin-right:15px;">Replay</button>'
    ].join('');
}

window.operateEvents = {
    'click .Play': function (e, value, row, index) {
        //$.get("http://192.168.99.178/rf_transmit", "num="+index);
        $.post("/rf_transmit", 'num=' + index);
        //alert(index);
    }
};


function refresh_rflist() {
    $.ajax({
        type: "GET",
        url: "/rf.json",
        dataType: "json",

        success: function (msg) {
            //var data=msg.rf[1].f;
            $('#rftable').bootstrapTable({
                height: 350,
                silent: true,
                columns: [{
                    field: 'f',
                    title: 'Freq'
                }, {
                    field: 'p',
                    title: 'Pac'
                }, {
                    field: 'm',
                    title: 'Modu'
                }, {
                    field: 'func',
                    title: 'func'
                }, {
                    field: 'd',
                    title: 'data'
                }, {
                    field: 'Button',
                    title: 'Play',
                    events: operateEvents,
                    formatter: operateFormatter
                    //events:,
                    //formatter:,
                }],
                data: msg.rf
            });
        },
        error: function () {
            alert("eeror");
        }
    });
}


function rflist() {
    $.ajax({
        type: "GET",
        url: "/rf.json",
        dataType: "json",

        success: function (msg) {
            //var data=msg.rf[1].f;
            $('#rftable').bootstrapTable({
                height: 350,
                columns: [{
                    field: 'f',
                    title: 'Freq'
                }, {
                    field: 'p',
                    title: 'Pac'
                }, {
                    field: 'm',
                    title: 'Modu'
                }, {
                    field: 'func',
                    title: 'func'
                }, {
                    field: 'd',
                    title: 'data'
                }, {
                    field: 'Button',
                    title: 'Play',
                    events: operateEvents,
                    formatter: operateFormatter
                    //events:,
                    //formatter:,
                }],
                data: msg.rf
            });
        },
        error: function () {
            alert("eeror");
        }
    });
}


function render() {
    fetch('/ajax_nfc').then(function (res) {
        return res;
    }).then(function (res) {
        return res.text();
    }).then(function (data) {
        //document.getElementById("result").innerHTML = data;
        //alert(data);
        if (data) {
            document.getElementById("nfc_idprint").innerHTML = data;
            $('#nfc_action').show();
        }

    })

    fetch('/ajax_rf_test').then(function (res) {
        return res;
    }).then(function (res) {
        return res.text();
    }).then(function (data) {
        //document.getElementById("result").innerHTML = data;
        //alert(data);
        if (data) {
            document.getElementById("rf_print").innerHTML = data;
            $('#rf_action').show();
            //$("#rftable").bootstrapTable('removeAll');
            //rflist();
        }

    })
    fetch('/ajax_rf_24l01').then(function (res) {
        return res;
    }).then(function (res) {
        return res.text();
    }).then(function (data) {
        if (data) {
            document.getElementById("rf_print_24l01").innerHTML = data;
            $('#rf_action_24l01').show();
            // $("#rftable").bootstrapTable('removeAll');
        }

    })
}

setInterval(render, 1000);

function render() {
    var ajax = new XMLHttpRequest();
    ajax.open('get', 'ajax_nfc');
    ajax.send();
    ajax.onreadystatechange = function () {
        if (ajax.readyState == 4 && ajax.status == 200) {
            var data = ajax.responseText.text()
            if (data) {
                document.getElementById("nfc_idprint").innerHTML = data;
                document.getElementById('nfc_action').style.display = 'block';
            }
        }
    }

    ajax.open('get', 'ajax_rf_test');
    ajax.send();
    ajax.onreadystatechange = function () {
        if (ajax.readyState == 4 && ajax.status == 200) {
            var data = ajax.responseText.text()
            if (data) {
                document.getElementById("rf_print").innerHTML = data;
                document.getElementById('rf_print').style.display = 'block';
            }
        }
    }

    ajax.open('get', 'ajax_rf_24l01');
    ajax.send();
    ajax.onreadystatechange = function () {
        if (ajax.readyState == 4 && ajax.status == 200) {
            var data = ajax.responseText.text()
            if (data) {
                document.getElementById("ajax_rf_24l01").innerHTML = data;
                document.getElementById('ajax_rf_24l01').style.display = 'block';
            }
        }
    }
}