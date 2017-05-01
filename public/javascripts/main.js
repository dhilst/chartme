$(() => {
    'use strict';

    var data = {
        labels: [],
        datasets: [{
            label: '',
            data: [],
        }],
    };

    const $canvas = $('#mychart');
    var graph = new Chart($canvas, {
        type: 'line',
        data: data,
        options: {
            resposive: true,
            maintainAspectRatio: false,
        }
    });

    var socket = io();
    socket.on('newdata', function(newData) {
        data.labels.push('');
        data.datasets[0].data.push(newData);
        graph.update();
    });
})
