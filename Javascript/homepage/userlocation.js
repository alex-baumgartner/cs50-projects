function returnGeolocation() {
    navigator.geolocation.getCurrentPosition(function(position) {
        let coordinates = "Your coordinates are: " + position.coords.latitude + ", " + position.coords.longitude;
        alert(coordinates);
    });
}