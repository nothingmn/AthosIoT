<template>
  <div class="content">
    <div class="container-fluid">
      <div class="row">
        <div class="col-md-6" v-for="(card, index) in cards" :key="index">
          <component  :is="card['card-type']" :device="card.card.device">
          </component>
        </div>
      </div>
    </div>
  </div>
</template>
<script>
  import DeviceView from './Admin/DeviceView.vue'
  import Vue from 'vue'

  export default {
    components: {
      DeviceView
    },
      data() {
        return {
          cards: [
          ]
        }
    },
      mounted() {    
        var vm = this;    
        var sock = Vue.prototype.$socket;
        sock.send(JSON.stringify({ action : "get-devices" }));
        console.log("get-devices sent");

        sock.onmessage = function(event) {
          if(event && event.data) {
            var msg = JSON.parse(event.data);
            if(msg.action && msg.action === "devices-list") {
              vm.cards = [];
              for(var d in msg.devices) {
                var device = msg.devices[d];                

                if(device && device.last && device.last.timeStamp) {
                  device.last.timeStamp = (new Date(device.last.timeStamp));
                }

                var exists = false;

                if(vm.cards) {
                  for(var c in vm.cards) {
                    var card = vm.cards[c];
                    if(card && card.card && card.card.device && card.card.device.deviceid && card.card.device.deviceid == device.deviceid) {
                      exists = true;
                      card.card.device = device;
                    }
                  }
                }
                if(!exists) {
                var card = {'card': { device : device }, 'card-type' : DeviceView};
                vm.cards.push(card);
                }
              }
            }          
        }
        };
      },   
      methods: {
        // sendWifiToServer () {
        //   var wifi = {
        //     action : "update-wifi",
        //     ssid : this.wifi.ssid,
        //     password : this.wifi.password
        //   }
        //   Vue.prototype.$socket.send(JSON.stringify(wifi));
        //   alert("Your wifi settings have been saved.")
        // }
        // Make the function wait until the connection is made...
    }
  }

</script>
<style>

</style>
