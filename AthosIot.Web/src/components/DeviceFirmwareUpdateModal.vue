<template>
    <modal name="device-firmware-update-modal" :adaptive="adaptive" :height="height"  @before-open="beforeOpen">
    <div  style="overflow-y: scroll; height:400px;">
      <card>
        <h4 slot="header" class="card-title">Update {{device.name}} ({{device.deviceid}}) firmware </h4>
        <div class="row">
          <div class="col-lg-12">
            Please choose the firmware you wish to apply to this device:
          </div>
        </div>
        <div class="row">
            <div class="col-md-12">
                <ul list-group class="list-group">
                    <li v-for="o in firmware"
                        class="list-group-item list-group-item-action flex-column align-items-start">
                        {{o}}&nbsp;&nbsp;
                        <button type="submit" class="btn btn-secondary  float-right" @click="updateFirmware(o)">
                        Apply
                        </button>
                    </li>
                </ul>
            </div>
          </div>
        <div class="row">
          <div class="col-lg-12">
            <br />
            <br />
            <button type="submit" class="btn btn-secondary  float-right" @click="hide()">
            Done
            </button>
          </div>
      </div>
      </card>
    </div>
    </modal>
</template>
<script>
import Card from 'src/components/Cards/Card.vue'
import Vue from 'vue'

export default {
  components: {
      Card
  },  
  name: 'device-firmware-update-modal',  
  data () {
    return {
      firmware : [],
      device : {},
      adaptive : true,
      height : 500
    }
  },
  methods: {
    beforeOpen (event) {
      this.device = event.params.model.device;
      var vm = this;    

      var sock = Vue.prototype.$socket;
      sock.send(JSON.stringify({ action : "get-firmware" }));
      sock.onmessage = function(event) {          
        if(event && event.data) {
          var msg = JSON.parse(event.data);

          if(msg.action && msg.action === "firmware-list") {
            vm.firmware = msg.list;
            console.log(this.firmware);        
          }
        }
      };  
    },
    hide() {
        this.$modal.hide('device-firmware-update-modal');
    },
    updateFirmware(o){
        var send =  { action : "update-device-firmware", deviceid : this.device.deviceid, path : o  };
        console.log("object model:", send);
        Vue.prototype.$socket.send(JSON.stringify(send));
        this.$modal.hide('device-firmware-update-modal');
    }
  }
}
</script>