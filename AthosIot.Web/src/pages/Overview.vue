<template>
  <div class="content">
    <div class="container-fluid">
      <div class="row">
        <div class="col-xl-3 col-md-6">
            <card>
              <h4 slot="header" class="card-title">Links</h4>
              <div class="row">
                <div class="col-lg-12">
                  <ul list-group class="list-group">
                    <li class="list-group-item list-group-item-action flex-column align-items-start">
                      <a href="#" @click.prevent="nav(':81')">RaspAP Manager</a>
                    </li>
                    <li class="list-group-item list-group-item-action flex-column align-items-start">
                      <a href="#" @click.prevent="nav(':1880')">Node-Red</a>
                    </li>
                  </ul>
                </div>
              </div>
            </card>
        </div>
      </div>
      <div class="row">
       <div class="col-md-8">
          <LineChartContainer name="chart-js-card" :chart-data="datacollection"></LineChartContainer>          
        </div>        
      </div>       
        </div>
    </div>
  </div>
</template>
<script>
  import LineChartContainer from 'src/components/Cards/LineChartContainer.vue'
  import Vue from 'vue'


  export default {
    components: {
      LineChartContainer,
    },
    data () {
      return {
        datacollection: null,
      }
    },
    created() {
        var vm = this;    
        var sock = Vue.prototype.$socket;
        sock.send(JSON.stringify({ action : "get-devices" }));
        console.log("get-devices sent");

        sock.onmessage = function(event) {          
          if(event && event.data) {
            var msg = JSON.parse(event.data);

            if(msg.action && msg.action === "devices-list") {

              for(var d in msg.devices) {
                var device = msg.devices[d];                
                if(device.last.temp) {                  
                  if(device && device.last && device.last.timeStamp) {
                    device.last.timeStamp = (new Date(device.last.timeStamp));
                  }
                  if(device.last.raw) delete device.last.raw;
                  if(device.last.since) delete device.last.since;                  
              }
            }
          }
        }
      }
    },
    methods : {
      nav(path) {
        path = "http://" + window.location.hostname + path;
        window.location = path;
      }
      
    },

  }
</script>
<style>

</style>
