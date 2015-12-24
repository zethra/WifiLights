var state = false, onColor='#35a1ff', offColor='#707880';

function on() {
    $.ajax({
        type: 'POST',
        url: '/ctrl',
        data: {
            message: 'on'
        },
        success: function(response) {
            console.log(response);
            updateUI();
        }
    })
}

function off() {
    $.ajax({
        type: 'POST',
        url: '/ctrl',
        data: {
            message: 'off'
        },
        success: function(response) {
            console.log(response);
            updateUI();
        }
    })
}

function toggle() {
    $.ajax({
        type: 'POST',
        url: '/ctrl',
        data: {
            message: 'toggle'
        },
        success: function(response) {
            console.log(response);
            updateUI();
        }
    })
}

function updateUI() {
    $.ajax({
        type: 'GET',
        url: '/state',
        success: function(response) {
            state = (response == 'true');
            var button = $('#toggle').find('a');
            if(state) {
                button.css('color', onColor);
            } else {
                button.css('color', offColor);
            }
        }
    });
}

$(document).ready(function(){
    updateUI();
    setInterval(updateUI, 1000);
});