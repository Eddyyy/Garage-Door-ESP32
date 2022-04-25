const char MAIN_page[200] PROGMEM = R"=====(
<!doctype html>
<html lang="en">
    <head>
        <!-- Required meta tags -->
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">

        <!-- Bootstrap CSS -->
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">

        <title>Hello, world!</title>
    </head>
    <script charset="utf-8">
        setInterval(function() {
            // Call a function repetatively with 2 Second interval
            getData();
        }, 2000); //2000mSeconds update rate
          
        function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.addEventListener("load", function() {
                if (this.status == 200) {
                    var initial_door_state =
                       document.getElementById("garage_state")
                    var alert_div = document.getElementById("door_alert");
                    var curr_alert_class = alert_div.className

                    // Modify Alert box
                    if (this.responseText === "down") {
                        alert_div.className = "alert alert-warning";
                    } else if (this.responseText === "up") {
                        alert_div.className = "alert alert-danger";
                    }

                    // Hide spinner if changed
                    if (curr_alert_class !== alert_div.className ||
                      !initial_door_state.innerHTML) {
                        document.getElementById("spinner").hidden = true;
                    }

                    // Display down or up
                    initial_door_state.innerHTML = this.responseText;
                }
            });
            xhttp.open("GET", "check_garage_door", true);
            xhttp.send();
        }

        function toggleGarage() {
        var xhttp = new XMLHttpRequest();
        document.getElementById("spinner").hidden = false;

        //xhttp.setRequestHeader("Content-type","application/x-www-form-urlencoded");
        xhttp.open("POST", "toggle_garage_door", true);
        xhttp.send();
        }

    </script>

    <h3>Our Flask Buttons<h3/>
    <div id="door_alert" class="alert alert-warning" role="alert">
        The door is <span id="garage_state"></span>
        <div class="spinner-border" role="status" id="spinner">
            <span class="visually-hidden">Loading...</span>
        </div>
    </div>
    <!-- <form method="post" action="" onsubmit="toggleGarage();return false;"> -->
        <button class="btn btn-primary" onclick=toggleGarage() />Toggle Garage</button>
    <!-- </form> -->
)=====";
