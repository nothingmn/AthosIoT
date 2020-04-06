<template>
    <modal name="rename-device-modal"  @before-open="beforeOpen">
      <card>
        <h4 slot="header" class="card-title">Rename Device</h4>
        <div class="row">
          <div class="col-lg-12">
            Please enter the new name for the device:{{device.id}}
          </div>
        </div>
        <div class="row">
          <div class="col-lg-12">
            <input v-model="device.name" placeholder="Device Name">
          </div>
        </div>
        <div class="row">
          <div class="col-lg-12">
            <button type="submit" class="btn btn-secondary  float-right" @click="hide()">
            Cancel
            </button>
            <button type="submit" class="btn btn-secondary  float-right" @click="device_rename()">
            Save
            </button>

          </div>
      </div>
      </card>
    </modal>
</template>
<script>
import Card from 'src/components/Cards/Card.vue'
import Vue from 'vue'

export default {
  components: {
      Card
  },  
  name: 'rename-device-modal',  
  data () {
    return {
      device : {}
    }
  },
  methods: {
    beforeOpen (event) {
      this.device = event.params.model.device;
    
    },
    hide() {
        this.$modal.hide('rename-device-modal');
    },
    device_rename(){
        var send =  { action : "rename-device", deviceid : this.device.deviceid, name: this.device.name };
        console.log("object model:", send);
        Vue.prototype.$socket.send(JSON.stringify(send));
        this.$modal.hide('rename-device-modal');
    }
  }
}
</script>