// Initialize the Google Map.
function initMap() {
  // Create the map.
  const map = new google.maps.Map(document.getElementById('map'), {
    zoom: 5,
    center: {lat: 40, lng: -95},
  });

  // Load the stores GeoJSON onto the map.
  map.data.loadGeoJson('locations.json', {idPropertyName: 'locationid'});

  // Define the custom marker icons, using the store's "category".
  map.data.setStyle((feature) => {
    return {
      icon: {
        url: `img/icon_${feature.getProperty('season')}.png`,
        scaledSize: new google.maps.Size(64, 64),
      },
    };
  });

  const infoWindow = new google.maps.InfoWindow();

  // Show the information for a store when its marker is clicked.
  map.data.addListener('click', (event) => {
    const location = event.feature.getProperty('location');
    const state = event.feature.getProperty('state');
    const season = event.feature.getProperty('season');
    const desc_1 = event.feature.getProperty('desc_1');
    const desc_2 = event.feature.getProperty('desc_2');
    const desc_3 = event.feature.getProperty('desc_3');
    const position = event.feature.getGeometry().get();
    const content = `
      <div style="margin-bottom:20px;">
        <h2>${location}, ${state}</h2>
      </div>
      <div id="carouselExampleCaptions" class="carousel slide" data-ride="carousel" style="margin-bottom:20px;">
        <ol class="carousel-indicators">
          <li data-target="#carouselExampleCaptions" data-slide-to="0" class="active"></li>
          <li data-target="#carouselExampleCaptions" data-slide-to="1"></li>
          <li data-target="#carouselExampleCaptions" data-slide-to="2"></li>
        </ol>
        <div class="carousel-inner">
          <div class="carousel-item active">
            <img src="img/${location}_1.jpg" class="d-block w-100" alt="...">
            <div class="carousel-caption d-none d-md-block">
              <h5>${state}</h5>
              <p>${desc_1}</p>
            </div>
          </div>
          <div class="carousel-item">
            <img src="img/${location}_2.jpg" class="d-block w-100" alt="...">
            <div class="carousel-caption d-none d-md-block">
              <h5>${state}</h5>
              <p>${desc_2}</p>
            </div>
          </div>
          <div class="carousel-item">
            <img src="img/${location}_3.jpg" class="d-block w-100" alt="...">
            <div class="carousel-caption d-none d-md-block">
              <h5>${state}</h5>
              <p>${desc_3}</p>
            </div>
          </div>
        </div>
        <a class="carousel-control-prev" href="#carouselExampleCaptions" role="button" data-slide="prev">
          <span class="carousel-control-prev-icon" aria-hidden="true"></span>
          <span class="sr-only">Previous</span>
        </a>
        <a class="carousel-control-next" href="#carouselExampleCaptions" role="button" data-slide="next">
          <span class="carousel-control-next-icon" aria-hidden="true"></span>
          <span class="sr-only">Next</span>
        </a>
      </div>
      `;
    infoWindow.setContent(content);
    infoWindow.setPosition(position);
    infoWindow.setOptions({pixelOffset: new google.maps.Size(0, -30)});
    infoWindow.open(map);
  });
}

//turns the joke into a loading bar and removes button
function getAnswer() {
  document.getElementById('joke').innerHTML = `
  <button class="btn btn-primary" type="button" disabled>
  <span class="spinner-border spinner-border-sm" role="status" aria-hidden="true"></span>
  Loading...
  </button>
  `;
  document.getElementById('answer').style.display = "none";
}