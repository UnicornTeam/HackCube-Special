var warning_action = document.getElementById('Warning_action')
warning_action.addEventListener('click', function (e) {
    this.style.display = 'none'
});
var nfc_action = document.getElementById('nfc_action')
nfc_action.addEventListener('click', function (e) {
    this.style.display = 'none'
});
var rf_action = document.getElementById('rf_action')
rf_action.addEventListener('click', function (e) {
    this.style.display = 'none'
});
var rf_action_24l01 = document.getElementById('rf_action_24l01')
rf_action_24l01.addEventListener('click', function (e) {
    this.style.display = 'none'
});


function render() {
    var a = new XMLHttpRequest();
    a.open('get', 'ajax_nfc');
    a.send();
    a.onreadystatechange = function () {
        if (a.readyState == 4 && a.status == 200) {
            var data = a.responseText
            if (data) {
                document.getElementById("nfc_idprint").innerHTML = data;
                document.getElementById('nfc_action').style.display = 'block';
                if (typeof nfclist === 'function') {
                    nfclist()
                }
            }
        }
    }
    b = new XMLHttpRequest();
    b.open('get', 'ajax_rf_test');
    b.send();
    b.onreadystatechange = function () {
        if (b.readyState == 4 && b.status == 200) {
            var data = b.responseText
            if (data) {
                document.getElementById("rf_print").innerHTML = data;
                document.getElementById('rf_action').style.display = 'block';
                if (typeof rflist === 'function') {
                    rflist()
                }
            }
        }
    }
    c = new XMLHttpRequest();
    c.open('get', 'ajax_rf_24l01');
    c.send();
    c.onreadystatechange = function () {
        if (c.readyState == 4 && c.status == 200) {
            var data = c.responseText
            if (data) {
                document.getElementById("rf_print_24l01").innerHTML = data;
                document.getElementById('rf_action_24l01').style.display = 'block';
                if (typeof rflist === 'function') {
                    rflist()
                }
            }
        }
    }


}

setInterval(render, 1000);
