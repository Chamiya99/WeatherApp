
// Get references to the canvas elements
var temperatureCtx = document.getElementById('temperatureChart').getContext('2d');
var humidityCtx = document.getElementById('humidityChart').getContext('2d');
var pressureCtx = document.getElementById('pressureChart').getContext('2d');

// Extract data from the table and prepare it for the charts
var table = document.querySelector('table');
var labels = [];
var temperatureData = [];
var humidityData = [];
var pressureData = [];

var rows = table.getElementsByTagName('tbody')[0].getElementsByTagName('tr');
for (var i = 0; i < rows.length; i++) {
    var cols = rows[i].getElementsByTagName('td');
    labels.push(cols[0].textContent);
    temperatureData.push(parseFloat(cols[1].textContent));
    humidityData.push(parseFloat(cols[2].textContent));
    pressureData.push(parseFloat(cols[3].textContent));
}

// Create separate charts for each data type
var temperatureChart = new Chart(temperatureCtx, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [
            {
                label: 'Temperature',
                data: temperatureData,
                borderColor: 'rgba(255, 99, 132, 1)',
                fill: false
            }
        ]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

var humidityChart = new Chart(humidityCtx, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [
            {
                label: 'Humidity',
                data: humidityData,
                borderColor: 'rgba(75, 192, 192, 1)',
                fill: false
            }
        ]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

var pressureChart = new Chart(pressureCtx, {
    type: 'line',
    data: {
        labels: labels,
        datasets: [
            {
                label: 'Pressure',
                data: pressureData,
                borderColor: 'rgba(54, 162, 235, 1)',
                fill: false
            }
        ]
    },
    options: {
        scales: {
            y: {
                beginAtZero: true
            }
        }
    }
});

//ON/OFF State req handle
function updateSystemState(checkbox) {
    var recordId = $(checkbox).data('record-id');
    var newState = checkbox.checked ? 1 : 0;

    jQuery.ajax({
        type: 'POST',
        url: 'updateSystemState.php', // Create a PHP script for updating the database
        data: { recordId: recordId, newState: newState },
        success: function (response) {
        console.log('System state updated successfully');
        },
        error: function () {
            console.error('Failed to update system state');
        }
    });
}
