<template>
  <card>
    <h4 slot="header" class="card-title">Edit Node WIFI Setting</h4>
    <form>
      <div class="row">
        <div class="col-md-6">
          <base-input type="text"
                    label="SSID"
                    placeholder="Wifi SSID"
                    v-model="wifi.ssid">
          </base-input>
        </div>
        <div class="col-md-6">
          <base-input type="password"
                    label="Password"
                    placeholder="Wifi Password"
                    v-model="wifi.password">
          </base-input>
        </div>
      </div>
      <div class="text-center">
        <button type="submit" class="btn btn-info btn-fill float-right" @click.prevent="sendWifiToServer">
          Update Wifi
        </button>
      </div>
      <div class="clearfix"></div>
    </form>
  </card>
</template>
<script>
  import Card from 'src/components/Cards/Card.vue'
  import Vue from 'vue'


export default {
    components: {
      Card
    },
    data () {
      return {
          wifi : {
            ssid: '',
            password: ''
          }
      }
    },
      mounted() {    
        var vm = this;    
        var sock = Vue.prototype.$socket;
        sock.send(JSON.stringify({ action : "get-wifi" }));
        console.log("get-wifi sent");
        sock.onmessage = function(event) {
          if(event && event.data) {
            var msg = JSON.parse(event.data);
            if(msg.action && msg.action === "update-wifi") {
              if(msg.ssid) vm.wifi.ssid = msg.ssid;
              if(msg.password) vm.wifi.password = msg.password;
            }
          }
        };
      } ,   
      methods: {
        sendWifiToServer () {
          var wifi = {
            action : "update-wifi",
            ssid : this.wifi.ssid,
            password : this.wifi.password
          }
          Vue.prototype.$socket.send(JSON.stringify(wifi));
          alert("Your wifi settings have been saved.")
        }
        // Make the function wait until the connection is made...
    }
  }

</script>
<style>

</style>
